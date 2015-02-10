CC=gcc
CFLAGS=-O2 -Wall -Werror -fPIC -pedantic
LDFLAGS=-O2 -Wall -Werror -fPIC -pedantic -lpthread
COMMON=server.o csapp.o req_handler.o serve_static.o

all: format baseline optimized cgi

baseline: serve_dynamic_baseline.o $(COMMON)
	$(CC) $(LDFLAGS) -o $@ $^

optimized: serve_dynamic_optimized.o code_cache.o $(COMMON)
	$(CC) $(LDFLAGS) -ldl -o $@ $^

cgi:
	(cd cgi-bin; make)

.PHONY: clean format

clean:
	-rm -f *.o baseline optimized *~
	(cd cgi-bin; make clean)

format:
	-clang-format -i *.{c,h}
