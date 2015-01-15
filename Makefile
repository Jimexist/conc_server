CC = gcc
CFLAGS = -O0 -g -Wall -I .
LDFLAGS = -lpthread

all: format baseline cgi
	
format:
	clang-format -i *.{c,h}

baseline: baseline.o csapp.o
	$(CC) -o baseline baseline.o csapp.o $(CFLAGS) $(LDFLAGS)

cgi:
	(cd cgi-bin; make)

clean:
	rm -f *.o tiny *~
	(cd cgi-bin; make clean)

