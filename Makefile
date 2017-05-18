CC=clang
CC-FLAGS=-Wall
CC-DEBUGFLAGS=-g
CC-OPTFLAGS=-O3 -march=native
LD-FLAGS=-lm -lreadline
RM=rm -f

.PHONY: all clean debug
.DEFAULT_GOAL := all

eratosthenes.o:	eratosthenes.h eratosthenes.c
	$(CC) $(CC-FLAGS) $(CC-OPTFLAGS) -o eratosthenes.o -c eratosthenes.c

driver.o:	driver.c
	$(CC) $(CC-FLAGS) $(CC-OPTFLAGS) -o driver.o -c driver.c

eratosthenes:	driver.o eratosthenes.o
	$(CC) $(CC-FLAGS) -o eratosthenes driver.o eratosthenes.o $(LD-FLAGS)

eratosthenes-debug.o:	eratosthenes.h eratosthenes.c
	$(CC) $(CC-FLAGS) $(CC-DEBUGFLAGS) -o eratosthenes-debug.o -c eratosthenes.c

driver-debug.o:	driver.c
	$(CC) $(CC-FLAGS) $(CC-DEBUGFLAGS) -o driver-debug.o -c driver.c

eratosthenes-debug:	driver-debug.o eratosthenes-debug.o
	$(CC) $(CC-FLAGS) -o eratosthenes-debug driver-debug.o eratosthenes-debug.o $(LD-FLAGS)

all: eratosthenes

debug: eratosthenes-debug

clean:
	$(RM) eratosthenes.o driver.o eratosthenes eratosthenes-debug.o driver-debug.o eratosthenes-debug
