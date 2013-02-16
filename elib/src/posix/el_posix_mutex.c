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
#include <pthread.h>
#include <errno.h>
#include <stdlib.h>
#include "../../inc/el_error.h"
#include "../../inc/el_mutex.h"


struct el_mutex_s {
  pthread_mutex_t mutex;
};



el_mutex_t* 
el_mutex_create(void) 
{
  el_mutex_t* mutex = (el_mutex_t*)malloc(sizeof(*mutex));
  
  if (NULL != mutex)
    pthread_mutex_init(&mutex->mutex, NULL);

  return mutex;
}

void 
el_mutex_delete(el_mutex_t** mutex) 
{
  if (NULL != *mutex) {
    pthread_mutex_destroy(&(*mutex)->mutex);
    free(*mutex);
    *mutex = NULL;
  }
}

void 
el_mutex_lock(el_mutex_t* mutex) 
{
  if (NULL != mutex) 
    pthread_mutex_lock(&mutex->mutex);
}

int 
el_mutex_trylock(el_mutex_t* mutex)
{
  if (NULL != mutex) {
    if (0 == pthread_mutex_trylock(&mutex->mutex))
      return EL_OK;
  }

  return EL_NO;
}

void 
el_mutex_unlock(el_mutex_t* mutex) 
{
  if (NULL != mutex) 
    pthread_mutex_unlock(&mutex->mutex);
}
