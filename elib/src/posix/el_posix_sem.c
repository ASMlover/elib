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
#include "../../inc/el_sem.h"



int32_t 
el_sem_init(el_sem_t* sem, uint32_t value)
{
  return sem_init(sem, 0, value);
}

void 
el_sem_destroy(el_sem_t* sem)
{
  if (0 != sem_destroy(sem))
    abort();
}

void 
el_sem_post(el_sem_t* sem)
{
  if (0 != sem_post(sem))
    abort();
}

void 
el_sem_wait(el_sem_t* sem)
{
  int ret;

  do {
    ret = sem_wait(sem);
  } while (-1 == ret && EINTR == errno);

  if (0 != ret)
    abort();
}

int32_t 
el_sem_trywait(el_sem_t* sem)
{
  int ret;

  do {
    ret = sem_trywait(sem);
  } while (-1 == ret && EINTR == errno);

  if (0 != ret && EAGAIN == errno)
    abort();

  return ret;
}
