CC = gcc
CFLAGS = -O2 -std=gnu11 -lm

cube: cube.c
	$(CC) $(CFLAGS) -o cube cube.c

.PHONY: install
install:
	cp cube /usr/local/bin/cube
