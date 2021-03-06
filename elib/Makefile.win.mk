# Copyright (c) 2013 ASMlover. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
#  * Redistributions of source code must retain the above copyright
#    notice, this list ofconditions and the following disclaimer.
#
#  * Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in
#    the documentation and/or other materialsprovided with the
#    distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

ELIB_OUT = evil.lib
ELIB_DIR = lib 
CC	= cl -c -nologo
AR	= lib -nologo
RM	= del 
RD	= rd /s /q
MD	= mkdir
CP	= copy
LINK	= link -nologo
CFLAGS	= -O2 -W3 -MD -GS -Zi -Fd"vc.pdb" -DNDEBUG\
	-D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_WARNINGS
ELIB_OBJS = el_error.obj\
	el_time.obj\
	el_win_time.obj\
	el_win_internal.obj\
	el_win_mutex.obj\
	el_win_rwlock.obj\
	el_win_sem.obj\
	el_win_cond.obj\
	el_win_thread.obj\
	el_win_spinlock.obj




all: lib 

lib: $(ELIB_OUT)

rebuild: clean all

install:
	if not exist $(ELIB_DIR) $(MD) $(ELIB_DIR)
	$(CP) $(ELIB_OUT) $(ELIB_DIR)

uninstall:
	if exist $(ELIB_DIR) $(RD) $(ELIB_DIR)

clean:
	$(RM) $(ELIB_OBJS) $(ELIB_OUT) *.pdb




$(ELIB_OUT): $(ELIB_OBJS)
	$(AR) -out:$(ELIB_OUT) $(ELIB_OBJS)

{./src}.c{}.obj:
	$(CC) $(CFLAGS) $<

{./src/win}.c{}.obj:
	$(CC) $(CFLAGS) $<
