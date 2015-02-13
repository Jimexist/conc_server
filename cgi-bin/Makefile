CC=gcc
CFLAGS=-O2 -Wall -Werror -fPIC -std=gnu99
LDFLAGS=-O2 -Wall -pthread -lm

EXE = adder multiplier sqrt rand time
OBJS := $(patsubst %,%.o, $(EXE))
SOS := $(patsubst %,lib%.so, $(EXE))

all: format $(SOS) $(EXE)

$(EXE): ../csapp.o cgi_main.o

lib%.so: %.o ../csapp.o
	$(CC) $(LDFLAGS) -shared $^ -o $@

.PHONY: clean format

clean:
	-rm -f $(EXE) *.o *.so

format:
	-clang-format -i *.c
