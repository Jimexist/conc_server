CC=gcc
CFLAGS=-O2 -Wall -Werror -fPIC -std=gnu99
LDFLAGS=-O2 -Wall -Werror -fPIC -lpthread
COMMON=server.o csapp.o req_handler.o serve_static.o code_cache.o

all: format baseline optimized cgi
	
test: all
	./compare.py

baseline: serve_dynamic_baseline.o $(COMMON)
	$(CC) $(LDFLAGS) -o $@ $^

optimized: serve_dynamic_optimized.o $(COMMON)
	$(CC) $(LDFLAGS) -ldl -o $@ $^

cgi:
	(cd cgi-bin; make)

.PHONY: clean format

clean:
	-rm -f *.o baseline optimized *~ wlog
	(cd cgi-bin; make clean)

format:
	-clang-format -i *.{c,h}
