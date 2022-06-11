CFLAGS=-std=c11 -g -static
SRCS:=$(wildcard *.c)
SRCS:=$(filter-out 9cc.c, $(SRCS))
HDRS:=$(wildcard *.h)
OBJS=$(SRCS:.c=.o)

mcc: $(OBJS) style
		$(CC) -o $@ $(OBJS) $(LDFLAGS)

style: $(SRCS) $(HDRS)
		cpplint $^

$(OBJS): mcc.h

test: mcc
		./test.sh

clean:
		rm -f mcc *.o *~ tmp*

.PHONY: test clean style