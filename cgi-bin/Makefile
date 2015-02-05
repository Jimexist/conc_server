CC=gcc
CFLAGS=-O2 -Wall -Werror -fPIC -Wextra -pedantic
DEBUGFLAGS=-O0
LDFLAGS=-Wall

all: format adder libadder.so
	
format:
	\clang-format -i *.c

adder: adder.o cgi_main.o
	
libadder.so: adder.o
	$(CC) -Wall -Werror -shared -o $@ $^

clean:
	\rm -f adder *.o *.so
