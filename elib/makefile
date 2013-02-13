ELIB_OUT = libevil.a
ELIB_DIR = lib 
CC	= gcc 
AR	= ar 
RM	= rm 
RD	= rm -rf
MD	= mkdir -p
CP	= cp 
CFLAGS	= -g -O2 -Wall 
ELIB_OBJS = ./src/el_error.o\
	./src/el_time.o\
	./src/posix/el_mutex.o\
	./src/posix/el_time.o


all: lib 

lib: $(ELIB_OUT)

rebuild: clean all

install:
	$(MD) $(ELIB_DIR)
	$(CP) $(ELIB_OUT) $(ELIB_DIR)

uninstall:
	$(RD) $(ELIB_DIR)

clean:
	$(RM) $(ELIB_OBJS) $(ELIB_OUT)




$(ELIB_OUT): $(ELIB_OBJS)
	$(AR) -cru $@ $^

$(ELIB_OBJS): %.o: %.c
	$(CC) -o $*.o -c $(CFLAGS) $^
