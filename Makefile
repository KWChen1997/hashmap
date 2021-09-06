CC = gcc
CFLAGS = -g -Wall
SRCS = $(wildcard *.c)
OBJS = $(patsubst %.c,%.o,$(SRCS))
HEADS = $(wildcard *.h)
PROGS = main

.PRECIOUS: $(OBJS)

all: $(PROGS)

$(PROGS):$(OBJS) $(HEADS)
	$(CC) $(CFLAGS) -o $@ $^
src:
	echo $(SRCS)
clean:
	rm -f *.o $(PROGS)
