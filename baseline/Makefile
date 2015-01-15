CC = gcc
CFLAGS = -O0 -g -Wall -I .

# This flag includes the Pthreads library on a Linux box.
# Others systems will probably require something different.
LIB = -lpthread

all: tiny cgi

tiny: tiny.o csapp.o
	$(CC) $(CFLAGS) -o tiny tiny.o csapp.o $(LIB)

tiny.o: tiny.c tiny.h
	$(CC) $(CFLAGS) -c tiny.c

csapp.o: csapp.c csapp.h
	$(CC) $(CFLAGS) -c csapp.c

cgi:
	(cd cgi-bin; make)

clean:
	rm -f *.o tiny *~
	(cd cgi-bin; make clean)

