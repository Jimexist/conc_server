CC=gcc
CFLAGS=-O2 -Wall -Werror -fPIC -pedantic
DEBUGFLAGS=-O0
LDFLAGS=-Wall

all: format adder libadder.so
	
csapp.o: ../csapp.c
	$(CC) $(CFLAGS) -c $^ -o $@
	
adder: adder.o cgi_main.o csapp.o
	$(CC) $(LDFLAGS) -o $@ $^
	
libadder.so: adder.o csapp.o
	$(CC) -Wall -Werror -shared -o $@ $^

.PHONY: clean format

clean:
	-rm -f adder *.o *.so

format:
	-clang-format -i *.c
