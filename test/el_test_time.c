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
#if (defined(_WIN32) || defined(_WIN64))
  #include <windows.h>
#else
  #include <unistd.h>
#endif
#include <stdio.h>
#include <el_time.h>
#include "el_test_header.h"


#if (defined(_WIN32) || defined(_WIN64))
  #define sleep(n)  Sleep(n * 1000)
#endif


static void 
test_localtime(void) 
{
  el_time_t time;
  el_localtime(&time);

  fprintf(stdout, 
    "Now Time %04d-%02d-%02d %02d:%02d:%02d:%03d\n", 
    time.year, time.mon, time.day, time.hour, 
    time.min, time.sec, time.millitm);
}

static void 
test_clock(void) 
{
  uint32_t beg = el_clock();
  uint32_t end;
  int i;

  for (i = 0; i < 10000; ++i)
    test_localtime();
  end = el_clock();
  fprintf(stdout, "time duration[%u ~ %u], use %u\n", beg, end, end - beg);

  beg = el_clock();
  sleep(1);
  end = el_clock();
  fprintf(stdout, "time duration[%u ~ %u], use %u\n", beg, end, end - beg);
}

void 
el_test_time(void* arg) 
{
  fprintf(stdout, "enter into function >> `%s`\n", __func__);

  test_clock();
}
