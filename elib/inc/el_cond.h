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
#ifndef __ELIB_COND_HEADER_H__
#define __ELIB_COND_HEADER_H__ 

#ifndef ELIB_COMMON
  #include "el_common.h"
#endif 

#if (defined(_WIN32) || defined(_WIN64))
  #include "./win32/el_win32_cond.h"
#else 
  #include "./posix/el_posix_cond.h"
#endif 

#include "el_mutex.h"

extern int32_t el_cond_init(el_cond_t* cond);
extern void el_cond_destroy(el_cond_t* cond);
extern void el_cond_signal(el_cond_t* cond);
extern void el_cond_broadcast(el_cond_t* cond);
extern void el_cond_wait(el_cond_t* cond, el_mutex_t* mutex);
extern int32_t el_cond_timedwait(el_cond_t* cond, 
    el_mutex_t* mutex, uint64_t timeout);

#endif  /* __ELIB_COND_HEADER_H__ */
