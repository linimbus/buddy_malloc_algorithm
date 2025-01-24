
CFLAGS = -g -Wall
CC = gcc

all: prog3.c
	$(CC) -o prog3 prog3.c $(CFLAGS)

clean:
	-$(RM) prog3 *.o