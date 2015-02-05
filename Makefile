CC=gcc
CFLAGS=-O0 -g -Wall -Werror
LDFLAGS=-lpthread

all: format baseline cgi

format:
	\clang-format -i *.{c,h}

baseline: baseline.o csapp.o req_handler.o

cgi:
	(cd cgi-bin; make)

clean:
	\rm -f *.o baseline *~
	(cd cgi-bin; make clean)

