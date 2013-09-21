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
#include <process.h>
#include "../../inc/el_error.h"
#include "../../inc/el_thread.h"


typedef struct el_context_s {
  void (*routine)(void* arg);
  void* arg;
} el_context_t;


static UINT WINAPI
el_thread_routine(void* ctx_arg)
{
  void (*routine)(void* arg);
  void* arg;
  el_context_t* ctx = (el_context_t*)ctx_arg;

  routine = ctx->routine;
  arg = ctx->arg;
  free(ctx);

  routine(arg);

  return 0;
}



int32_t 
el_thread_create(el_thread_t* tid, void (*routine)(void* arg), void* arg)
{
  el_context_t* ctx;

  if (NULL == (ctx = (el_context_t*)malloc(sizeof(*ctx)))) 
    return EL_NO;
  ctx->routine = routine;
  ctx->arg = arg;
  
  *tid = (HANDLE)_beginthreadex(NULL, 0, el_thread_routine, ctx, 0, NULL);
  if (NULL == *tid) {
    free(ctx);
    return EL_NO;
  }

  return EL_OK;
}

uint32_t 
el_thread_self(void)
{
  return (uint32_t)GetCurrentThreadId();
}

int32_t 
el_thread_join(el_thread_t* tid) 
{
  if (WaitForSingleObject(*tid, INFINITE)) {
    return EL_NO;
  }
  else {
    CloseHandle(*tid);
    *tid = NULL;
    return EL_OK;
  }
}
