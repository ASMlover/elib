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
#ifndef __ELIB_RWLOCK_HEADER_H__
#define __ELIB_RWLOCK_HEADER_H__ 

#ifndef ELIB_COMMON
# include "el_common.h"
#endif 

#if (defined(_WINDOWS_) || defined(_MSC_VER))
# include "./win32/el_win32_rwlock.h"
#elif (defined(__linux__))
# include "./posix/el_posix_rwlock.h"
#endif


extern int32_t el_rwlock_init(el_rwlock_t* rwlock);
extern void el_rwlock_destroy(el_rwlock_t* rwlock);
extern void el_rwlock_rdlock(el_rwlock_t* rwlock);
extern int32_t el_rwlock_tryrdlock(el_rwlock_t* rwlock);
extern void el_rwlock_rdunlock(el_rwlock_t* rwlock);
extern void el_rwlock_wrlock(el_rwlock_t* rwlock);
extern int32_t el_rwlock_trywrlock(el_rwlock_t* rwlock);
extern void el_rwlock_wrunlock(el_rwlock_t* rwlock);

#endif  /* __ELIB_RWLOCK_HEADER_H__ */
