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
#include <stdlib.h>
#include <errno.h>
#include "../../inc/el_error.h"
#include "../../inc/el_cond.h"


#undef NANOSEC
#define NANOSEC ((uint64_t)1e9)

static inline uint64_t 
el_hrtime(void)
{
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return (((uint64_t)ts.tv_sec) * NANOSEC + ts.tv_nsec);
}



int32_t 
el_cond_init(el_cond_t* cond)
{
  pthread_condattr_t attr;

  if (0 != pthread_condattr_init(&attr))
    return EL_NO;
  if (0 != pthread_condattr_setclock(&attr, CLOCK_MONOTONIC))
    goto __error2;

  if (0 != pthread_cond_init(cond, &attr))
    goto __error2;
  if (0 != pthread_condattr_destroy(&attr))
    goto __error;

  return EL_OK;


__error:
  pthread_cond_destroy(cond);
__error2:
  pthread_condattr_destroy(&attr);
  return EL_NO;
}

void 
el_cond_destroy(el_cond_t* cond)
{
  if (0 != pthread_cond_destroy(cond))
    abort();
}

void 
el_cond_signal(el_cond_t* cond)
{
  if (0 != pthread_cond_signal(cond))
    abort();
}

void 
el_cond_broadcast(el_cond_t* cond)
{
  if (0 != pthread_cond_broadcast(cond))
    abort();
}

void 
el_cond_wait(el_cond_t* cond, el_mutex_t* mutex) 
{
  if (0 != pthread_cond_wait(cond, mutex))
    abort();
}

int32_t 
el_cond_timedwait(el_cond_t* cond, el_mutex_t* mutex, uint64_t timeout)
{
  int result;
  struct timespec ts;

  timeout += el_hrtime();
  ts.tv_sec = timeout / NANOSEC;
  ts.tv_nsec = timeout % NANOSEC;
  result = pthread_cond_timedwait(cond, mutex, &ts);

  if (0 == result)
    return EL_OK;
  if (ETIMEDOUT == result)
    return EL_NO;

  abort();
  return EL_NO;
}
