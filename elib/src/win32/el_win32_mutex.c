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
 *    notice, this list of conditions and the following disclaimer in
 *  * Redistributions in binary form must reproduce the above copyright
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
#include <windows.h>
#include "../../inc/el_mutex.h"


struct el_mutex_s {
  CRITICAL_SECTION mutex;
};



static int 
mutex_init(el_mutex_t* mutex) 
{
  __try {
    InitializeCriticalSection(&mutex->mutex);
  }
  __except (STATUS_NO_MEMORY == GetExceptionCode()
    ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH) {
    return ERROR_OUTOFMEMORY;
  }

  return 0;
}

static int 
mutex_lock(el_mutex_t* mutex) 
{
  __try {
    EnterCriticalSection(&mutex->mutex);
  }
  __except (STATUS_INVALID_HANDLE == GetExceptionCode() 
    || STATUS_NO_MEMORY == GetExceptionCode()
    ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH) {
    return (STATUS_NO_MEMORY == GetExceptionCode()
      ? ERROR_OUTOFMEMORY : ERROR_INVALID_HANDLE);
  }

  return 0;
}





el_mutex_t* 
el_mutex_create(void) 
{
  el_mutex_t* mutex = (el_mutex_t*)malloc(sizeof(*mutex));

  if (NULL != mutex) 
    mutex_init(mutex);

  return mutex;
}

void 
el_mutex_delete(el_mutex_t** mutex) 
{
  if (NULL != *mutex) {
    DeleteCriticalSection(&(*mutex)->mutex);
    free(*mutex);
    *mutex = NULL;
  }
}

void 
el_mutex_lock(el_mutex_t* mutex) 
{
  if (NULL != mutex) 
    mutex_lock(mutex);
}

void 
el_mutex_unlock(el_mutex_t* mutex) 
{
  if (NULL != mutex) 
    LeaveCriticalSection(&mutex->mutex);
}
