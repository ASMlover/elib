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
#ifndef __ELIB_COMMON_HEADER_H__
#define __ELIB_COMMON_HEADER_H__ 

#ifndef ELIB_COMMON
#define ELIB_COMMON 


#if defined(_WINDOWS_) || defined(_MSC_VER)
# if (_MSC_VER < 1600)
#   if (_MSC_VER < 1300)
      typedef signed char       int8_t;
      typedef unsigned char     uint8_t;
      typedef signed short      int16_t;
      typedef unsigned short    uint16_t;
      typedef signed int        int32_t;
      typedef unsigned int      uint32_t;
#   else
      typedef signed __int8     int8_t;
      typedef unsigned __int8   uint8_t;
      typedef signed __int16    int16_t;
      typedef unsigned __int16  uint16_t;
      typedef signed __int32    int32_t;
      typedef unsigned __int32  uint32_t;
#   endif
    typedef signed __int64      int64_t;
    typedef unsigned __in64     uint64_t;

#   ifndef _W64
#     if (!defined(__midl) \
          && (defined(_X86_) || defined(_M_IX86)) \
          && _MSC_VER >= 1300)
#       define _W64 __w64
#     else
#       define _W64
#     endif
#   endif 

#   ifdef _WIN64 
      typedef signed __int64      intptr_t;
      typedef unsigned __int64    uintptr_t
#   else 
      typedef _W64 signed int     intptr_t;
      typedef _W64 unsigned int   uintptr_t;
#   endif
# else
#   include <stdint.h>
# endif 

# define inline   __inline
# define __func__ __FUNCTION__

#elif defined(__linux__)
# include <stdint.h>
#endif


#ifndef countof
# define countof(s)    (sizeof((s)) / sizeof(*(s)))
#endif


#endif  /* ELIB_COMMON */

#endif  /* __ELIB_COMMON_HEADER_H__ */
