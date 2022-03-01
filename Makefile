CFLAGS=-std=c11 -g -static
SRCS:=$(wildcard *.c)
SRCS:=$(filter-out 9cc.c, $(SRCS))
OBJS=$(SRCS:.c=.o)

mcc: $(OBJS)
		$(CC) -o $@ $(OBJS) $(LDFLAGS)

$(OBJS): mcc.h

test: mcc
		./test.sh

clean:
		rm -f mcc *.o *~ tmp*

.PHONY: test clean