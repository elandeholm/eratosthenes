CC=clang
CC-FLAGS=-Wall
CC-OPTFLAGS=-g#-O3 -march=native
LD-FLAGS=-lm -lreadline
RM=rm -f

eratosthenes.o:	eratosthenes.h eratosthenes.c
	$(CC) $(CC-FLAGS) $(CC-OPTFLAGS) -o eratosthenes.o -c eratosthenes.c

driver.o:	driver.c
	$(CC) $(CC-FLAGS) $(CC-OPTFLAGS) -o driver.o -c driver.c

eratosthenes:	driver.o eratosthenes.o
	$(CC) $(CC-FLAGS) -o eratosthenes driver.o eratosthenes.o $(LD-FLAGS)

clean:
	$(RM) eratosthenes.o driver.o eratosthenes
