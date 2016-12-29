CFLAGS = -O3 -Wall -march=native

CC = gcc

.SUFFIXES: .c .o

.c.o:
	$(CC) $(CFLAGS) -c $<

OBJS = timemath.o

SRCS = timemath.c

all: timemath

timemath: $(OBJS)
	$(CC) $(OBJS) -o timemath

clean:
	rm *.o timemath
