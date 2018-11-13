#
# This makefile is provided as an example.  It can be modified
# to work for assignment 3 by replacing the 'abc123' entries
# with your UTSA id.

# The 'run' command will only work if you
# have the appropriate text files in the same directory, and
# they have the same names
#


PROGRAMS = p4

CC = gcc
CFLAGS =

all: $(PROGRAMS)

tidy:
	rm -f ,* .,* *~ core a.out graphics.ps

clean:
	rm -f *.o *.a $(PROGRAMS)

run:
	./p4 -u p4Users.txt -c p4Command.txt

# *****************************
# compilations
cs1713p4Driver.o: cs1713p4Driver.c cs1713p4.h
	$(CC) $(CFLAGS) -c cs1713p4Driver.c

p4nra253.o: p4nra253.c cs1713p4.h
	$(CC) $(CFLAGS) -c p4nra253.c

# *****************************
# generate executable
p4: cs1713p4Driver.o p4nra253.o
	$(CC) $(CFLAGS) -o p4 cs1713p4Driver.o p4nra253.o
