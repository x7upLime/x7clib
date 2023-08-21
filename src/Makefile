CC = gcc
CFLAGS = -Wall -Wextra -fpic -ggdb
LIBTARGET = ../lib/libx7clib.so

X7CLIB_INSTALL_PATH = /usr/lib
X7CLIB_H_INSTALL_PATH = /usr/include

OBJS = ./x7ht/x7ht.o
HTARGETS = ./x7ht/x7ht.h

.PHONY: install clean

all: x7clib

x7clib: $(OBJS)
	$(CC) $(CFLAGS) -shared -o $(LIBTARGET) $(OBJS)

install:
	cp $(LIBTARGET) /usr/lib
	cp $(HTARGETS) /usr/include
	ldconfig

x7ht.o: ./x7ht/x7ht.c ./x7ht/x7ht.h
	$(CC) $(CFLAGS) -c ./x7ht/x7ht.c -o ./x7ht/x7ht.o

clean:
	rm $(OBJS) $(LIBTARGET)
