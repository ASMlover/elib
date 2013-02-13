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
#include <stdio.h>
#include <string.h>
#include <el_common.h>
#include "el_test_header.h"


typedef void (*callback_t)(void*);
typedef struct test_cmd_s {
  const char* cmd;
  callback_t  cb;
} test_cmd_t;


static const test_cmd_t s_cmds[] = {
  {"time", el_test_time}, 
};



#define EL_HELP_SHOW(cmd, module)\
  fprintf(stdout, "  %s\t%s\n", (cmd), "testcase of `" #module "` module")

static void 
el_test_helper(void* arg) 
{
  fprintf(stdout, 
    "testcases of evil library.\n"
    "  Usage: elib-test [options] ...\n\n"
    "  help\tdisplay all test command list\n");

  EL_HELP_SHOW("time", el_time);
}


int 
main(int argc, char* argv[]) 
{
  if (argc < 2) 
    el_test_helper(NULL);
  else {
    int called = 0;
    int i, count = countof(s_cmds);

    for (i = 0; i < count; ++i) {
#if (defined(_WIN32) || defined(_WIN64))
      if (0 == stricmp(argv[1], s_cmds[i].cmd)) {
#else
      if (0 == strcasecmp(argv[1], s_cmds[i].cmd)) {
#endif
        s_cmds[i].cb(NULL);
        called = 1;
        break;
      }
    }

    if (!called)
      el_test_helper(NULL);
  }

  return 0;
}
