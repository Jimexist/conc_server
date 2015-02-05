CC=gcc
CFLAGS=-O2 -Wall
LDFLAGS=-Wall

all: format adder
	
format:
	\clang-format -i *.c

adder: adder.o cgi_main.o

clean:
	\rm -f adder *.o
