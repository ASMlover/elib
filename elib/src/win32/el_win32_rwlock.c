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
#include "../../inc/el_common.h"
#include "../../inc/win32/el_win32_internal.h"
#include "../../inc/el_error.h"
#include "../../inc/el_rwlock.h"


static win32_version_t s_version;

#define SRWLOCK_API_VALID()\
  ((_MSC_VER >= 1600) && (s_version.major >= 6 && s_version.minor >= 1))


static inline int 
el_rwlock_srwlock_init(el_rwlock_t* rwlock)
{
#if (_MSC_VER >= 1600)
  InitializeSRWLock(&rwlock->srwlock);
#endif
  return EL_OK;
}

static inline void 
el_rwlock_srwlock_destroy(el_rwlock_t* rwlock)
{
  (void)rwlock;
}

static inline void 
el_rwlock_srwlock_rdlock(el_rwlock_t* rwlock)
{
#if (_MSC_VER >= 1600)
  AcquireSRWLockShared(&rwlock->srwlock);
#endif
}

static inline int 
el_rwlock_srwlock_tryrdlock(el_rwlock_t* rwlock)
{
#if (_MSC_VER >= 1600)
  if (!TryAcquireSRWLockShared(&rwlock->srwlock))
    return EL_NO;
#endif
  return EL_OK;
}

static inline void 
el_rwlock_srwlock_rdunlock(el_rwlock_t* rwlock) 
{
#if (_MSC_VER >= 1600)
  ReleaseSRWLockShared(&rwlock->srwlock);
#endif
}

static inline void 
el_rwlock_srwlock_wrlock(el_rwlock_t* rwlock)
{
#if (_MSC_VER >= 1600)
  AcquireSRWLockExclusive(&rwlock->srwlock);
#endif
}

static inline int 
el_rwlock_srwlock_trywrlock(el_rwlock_t* rwlock) 
{
#if (_MSC_VER >= 1600)
  if (!TryAcquireSRWLockExclusive(&rwlock->srwlock))
    return EL_NO;
#endif
  return EL_OK;
}

static inline void 
el_rwlock_srwlock_wrunlock(el_rwlock_t* rwlock)
{
#if (_MSC_VER >= 1600)
  ReleaseSRWLockExclusive(&rwlock->srwlock);
#endif
}

static inline int 
el_rwlock_self_rwlock_init(el_rwlock_t* rwlock)
{
  InitializeCriticalSection(&rwlock->self_rwlock.rd_mutex);
  InitializeCriticalSection(&rwlock->self_rwlock.wr_mutex);
  rwlock->self_rwlock.num_readers = 0;

  return EL_OK;
}

static inline void 
el_rwlock_self_rwlock_destroy(el_rwlock_t* rwlock)
{
  DeleteCriticalSection(&rwlock->self_rwlock.rd_mutex);
  DeleteCriticalSection(&rwlock->self_rwlock.wr_mutex);
}

static inline void 
el_rwlock_self_rwlock_rdlock(el_rwlock_t* rwlock)
{
  EnterCriticalSection(&rwlock->self_rwlock.rd_mutex);

  if (1 == ++rwlock->self_rwlock.num_readers)
    EnterCriticalSection(&rwlock->self_rwlock.wr_mutex);

  LeaveCriticalSection(&rwlock->self_rwlock.rd_mutex);
}

static inline int 
el_rwlock_self_rwlock_tryrdlock(el_rwlock_t* rwlock)
{
  int ret = EL_NO;

  if (!TryEnterCriticalSection(&rwlock->self_rwlock.rd_mutex))
    goto __quit;

  if (0 == rwlock->self_rwlock.num_readers)
    ret = TryEnterCriticalSection(&rwlock->self_rwlock.wr_mutex) 
      ? EL_OK : EL_NO;
  else
    ret = EL_OK;

  if (EL_OK == ret)
    ++rwlock->self_rwlock.num_readers;

  LeaveCriticalSection(&rwlock->self_rwlock.rd_mutex);

__quit:
  return ret;
}

static inline void 
el_rwlock_self_rwlock_rdunlock(el_rwlock_t* rwlock)
{
  EnterCriticalSection(&rwlock->self_rwlock.rd_mutex);

  if (0 == --rwlock->self_rwlock.num_readers)
    LeaveCriticalSection(&rwlock->self_rwlock.wr_mutex);

  LeaveCriticalSection(&rwlock->self_rwlock.rd_mutex);
}

static inline void 
el_rwlock_self_rwlock_wrlock(el_rwlock_t* rwlock)
{
  EnterCriticalSection(&rwlock->self_rwlock.wr_mutex);
}

static inline int 
el_rwlock_self_rwlock_trywrlock(el_rwlock_t* rwlock)
{
  if (TryEnterCriticalSection(&rwlock->self_rwlock.wr_mutex))
    return EL_OK;
  else
    return EL_NO;
}

static inline void 
el_rwlock_self_rwlock_wrunlock(el_rwlock_t* rwlock)
{
  LeaveCriticalSection(&rwlock->self_rwlock.wr_mutex);
}




int 
el_rwlock_init(el_rwlock_t* rwlock)
{
  s_version = *win32_get_version();

  if (SRWLOCK_API_VALID())
    return el_rwlock_srwlock_init(rwlock);
  else 
    return el_rwlock_self_rwlock_init(rwlock);
}

void 
el_rwlock_destroy(el_rwlock_t* rwlock)
{
  if (SRWLOCK_API_VALID())
    el_rwlock_srwlock_destroy(rwlock);
  else 
    el_rwlock_self_rwlock_destroy(rwlock);
}

void 
el_rwlock_rdlock(el_rwlock_t* rwlock)
{
  if (SRWLOCK_API_VALID())
    el_rwlock_srwlock_rdlock(rwlock);
  else 
    el_rwlock_self_rwlock_rdlock(rwlock);
}

int 
el_rwlock_tryrdlock(el_rwlock_t* rwlock) 
{
  if (SRWLOCK_API_VALID())
    return el_rwlock_srwlock_tryrdlock(rwlock);
  else
    return el_rwlock_self_rwlock_tryrdlock(rwlock);
}

void 
el_rwlock_rdunlock(el_rwlock_t* rwlock)
{
  if (SRWLOCK_API_VALID())
    el_rwlock_srwlock_rdunlock(rwlock);
  else 
    el_rwlock_self_rwlock_rdunlock(rwlock);
}

void 
el_rwlock_wrlock(el_rwlock_t* rwlock)
{
  if (SRWLOCK_API_VALID())
    el_rwlock_srwlock_wrlock(rwlock);
  else 
    el_rwlock_self_rwlock_wrlock(rwlock);
}

int 
el_rwlock_trywrlock(el_rwlock_t* rwlock)
{
  if (SRWLOCK_API_VALID())
    return el_rwlock_srwlock_trywrlock(rwlock);
  else 
    return el_rwlock_self_rwlock_trywrlock(rwlock);
}

void 
el_rwlock_wrunlock(el_rwlock_t* rwlock)
{
  if (SRWLOCK_API_VALID())
    el_rwlock_srwlock_wrunlock(rwlock);
  else 
    el_rwlock_self_rwlock_wrunlock(rwlock);
}
