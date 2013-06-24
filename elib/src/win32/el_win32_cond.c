/*
 * Copyright (c) 2013 ASMlover. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list ofconditions and the following disclaimer.
 *
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materialsprovided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#include "../../inc/win32/el_win32_internal.h"
#include "../../inc/el_error.h"
#include "../../inc/el_cond.h" 


static win32_version_t s_version;

#define CONDVAR_API_VALID()\
  ((_MSC_VER >= 1500) && (s_version.major >= 6 && s_version.minor >= 0))



static inline int32_t 
el_cond_condvar_init(el_cond_t* cond) 
{
#if (_MSC_VER >= 1500)
  InitializeConditionVariable(&cond->cond_var);
#endif
  return EL_OK;
}

static inline void 
el_cond_condvar_destroy(el_cond_t* cond) 
{
}

static inline void 
el_cond_condvar_signal(el_cond_t* cond) 
{
#if (_MSC_VER >= 1500)
  WakeConditionVariable(&cond->cond_var);
#endif
}

static inline void 
el_cond_condvar_broadcast(el_cond_t* cond)
{
#if (_MSC_VER >= 1500)
  WakeAllConditionVariable(&cond->cond_var);
#endif
}

static inline void 
el_cond_condvar_wait(el_cond_t* cond, el_mutex_t* mutex) 
{
#if (_MSC_VER >= 1500)
  if (!SleepConditionVariableCS(&cond->cond_var, mutex, INFINITE))
    abort();
#endif
}

static inline int32_t 
el_cond_condvar_timedwait(el_cond_t* cond, 
    el_mutex_t* mutex, uint64_t timeout)
{
#if (_MSC_VER >= 1500)
  if (SleepConditionVariableCS(&cond->cond_var, 
      mutex, (DWORD)(timeout / 1e6)))
    return EL_OK;
  if (ERROR_TIMEOUT == GetLastError())
    abort();
#endif 
  return EL_NO;
}

static inline int32_t 
el_cond_self_cond_init(el_cond_t* cond)
{
  cond->self_cond.waiters_count = 0;
  InitializeCriticalSection(&cond->self_cond.waiters_count_lock);
  cond->self_cond.signal_event = CreateEvent(NULL, FALSE, FALSE, NULL);
  if (NULL == cond->self_cond.signal_event)
    goto __error2;

  cond->self_cond.broadcast_event = CreateEvent(NULL, TRUE, FALSE, NULL);
  if (NULL == cond->self_cond.broadcast_event)
    goto __error;

  return EL_OK;


__error:
  CloseHandle(cond->self_cond.signal_event);
__error2:
  DeleteCriticalSection(&cond->self_cond.waiters_count_lock);
  return EL_NO;
}

static inline void 
el_cond_self_cond_destroy(el_cond_t* cond)
{
  if (!CloseHandle(cond->self_cond.broadcast_event))
    abort();
  if (!CloseHandle(cond->self_cond.signal_event))
    abort();
  DeleteCriticalSection(&cond->self_cond.waiters_count_lock);
} 

static inline void 
el_cond_self_cond_signal(el_cond_t* cond)
{
  BOOL have_waiters;

  EnterCriticalSection(&cond->self_cond.waiters_count_lock);
  have_waiters = cond->self_cond.waiters_count > 0;
  LeaveCriticalSection(&cond->self_cond.waiters_count_lock);

  if (have_waiters)
    SetEvent(cond->self_cond.signal_event);
} 

static inline void 
el_cond_self_cond_broadcast(el_cond_t* cond)
{
  BOOL have_waiters;

  EnterCriticalSection(&cond->self_cond.waiters_count_lock);
  have_waiters = cond->self_cond.waiters_count > 0;
  LeaveCriticalSection(&cond->self_cond.waiters_count_lock);

  if (have_waiters)
    SetEvent(cond->self_cond.broadcast_event);
}

static inline int32_t 
el_cond_self_cond_wait_helper(el_cond_t* cond, 
    el_mutex_t* mutex, DWORD timeout)
{
  DWORD result;
  BOOL last_waiter;
  HANDLE handles[2] = {
    cond->self_cond.signal_event, 
    cond->self_cond.broadcast_event
  };

  EnterCriticalSection(&cond->self_cond.waiters_count_lock);
  ++cond->self_cond.waiters_count;
  LeaveCriticalSection(&cond->self_cond.waiters_count_lock);

  el_mutex_unlock(mutex);
  
  result = WaitForMultipleObjects(2, handles, FALSE, timeout);
  EnterCriticalSection(&cond->self_cond.waiters_count_lock);
  --cond->self_cond.waiters_count;
  last_waiter = ((WAIT_OBJECT_0 + 1 == result)
      && (0 == cond->self_cond.waiters_count));
  LeaveCriticalSection(&cond->self_cond.waiters_count_lock);

  if (last_waiter)
    ResetEvent(cond->self_cond.broadcast_event);

  el_mutex_lock(mutex);

  if (WAIT_OBJECT_0 == result || (WAIT_OBJECT_0 + 1) == result)
    return EL_OK;
  if (WAIT_TIMEOUT)
    return EL_NO;

  abort();
  return EL_NO;
}

static inline void 
el_cond_self_cond_wait(el_cond_t* cond, el_mutex_t* mutex)
{
  if (EL_OK != el_cond_self_cond_wait_helper(cond, mutex, INFINITE))
    abort();
}

static inline int32_t 
el_cond_self_cond_timedwait(el_cond_t* cond, 
    el_mutex_t* mutex, uint64_t timeout)
{
  return el_cond_self_cond_wait_helper(cond, mutex, (DWORD)(timeout / 1e6));
}



int32_t
el_cond_init(el_cond_t* cond)
{
  s_version = *win32_get_version();

  if (CONDVAR_API_VALID())
    return el_cond_condvar_init(cond);
  else 
    return el_cond_self_cond_init(cond);
}

void 
el_cond_destroy(el_cond_t* cond)
{
  if (CONDVAR_API_VALID())
    el_cond_condvar_destroy(cond);
  else
    el_cond_self_cond_destroy(cond);
}

void 
el_cond_signal(el_cond_t* cond)
{
  if (CONDVAR_API_VALID())
    el_cond_condvar_signal(cond);
  else 
    el_cond_self_cond_signal(cond);
}

void 
el_cond_broadcast(el_cond_t* cond)
{
  if (CONDVAR_API_VALID())
    el_cond_condvar_broadcast(cond);
  else 
    el_cond_self_cond_broadcast(cond);
}

void 
el_cond_wait(el_cond_t* cond, el_mutex_t* mutex)
{
  if (CONDVAR_API_VALID())
    el_cond_condvar_wait(cond, mutex);
  else 
    el_cond_self_cond_wait(cond, mutex);
}

int32_t 
el_cond_timedwait(el_cond_t* cond, el_mutex_t* mutex, uint64_t timeout)
{
  if (CONDVAR_API_VALID())
    return el_cond_condvar_timedwait(cond, mutex, timeout);
  else 
    return el_cond_self_cond_timedwait(cond, mutex, timeout);
}
