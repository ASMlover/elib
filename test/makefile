BIN_OUT	= elib-test
BIN_DIR	= ../bin 
CC	= gcc
CFLAGS	+= -g -O2 -Wall 
RM	= rm 
MD	= mkdir -p
RD	= rm -rf
CP	= cp 
TEST_OBJS = test_main.o



all: bin 

bin: $(BIN_OUT)

rebuild: clean all 

install:
	$(MD) $(BIN_DIR)
	$(CP) $(BIN_OUT) $(BIN_DIR)

uninstall:
	$(RD) $(BIN_DIR)

clean:
	$(RM) $(TEST_OBJS) $(BIN_OUT)




$(BIN_OUT): $(TEST_OBJS)
	$(CC) -o $@ $^

$(TEST_OBJS): %.o: %.c
	$(CC) -o $*.o -c $(CFLAGS) $^
