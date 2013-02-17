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
#include <errno.h>
#include <stdlib.h>
#include "../../inc/el_error.h"
#include "../../inc/el_rwlock.h"



int 
el_rwlock_init(el_rwlock_t* rwlock)
{
  if (0 == pthread_rwlock_init(rwlock, NULL))
    return EL_OK;
  else 
    return EL_NO;
}

void 
el_rwlock_destroy(el_rwlock_t* rwlock)
{
  if (0 != pthread_rwlock_destroy(rwlock))
    abort();
}

void 
el_rwlock_rdlock(el_rwlock_t* rwlock)
{
  if (0 != pthread_rwlock_rdlock(rwlock))
    abort();
}

int 
el_rwlock_tryrdlock(el_rwlock_t* rwlock)
{
  int ret = pthread_rwlock_tryrdlock(rwlock);

  if (0 != ret && EBUSY != ret && EAGAIN != ret)
    abort();

  return (0 == ret ? EL_OK : EL_NO);
}

void 
el_rwlock_rdunlock(el_rwlock_t* rwlock)
{
  if (0 != pthread_rwlock_unlock(rwlock))
    abort();
}

void 
el_rwlock_wrlock(el_rwlock_t* rwlock)
{
  if (0 != pthread_rwlock_wrlock(rwlock))
    abort();
}

int 
el_rwlock_trywrlock(el_rwlock_t* rwlock)
{
  int ret = pthread_rwlock_trywrlock(rwlock);

  if (0 != ret && EBUSY != ret && EAGAIN != ret)
    abort();

  return (0 == ret ? EL_OK : EL_NO);
}

void 
el_rwlock_wrunlock(el_rwlock_t* rwlock)
{
  if (0 != pthread_rwlock_unlock(rwlock))
    abort();
}
