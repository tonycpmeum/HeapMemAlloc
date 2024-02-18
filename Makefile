CC = gcc
CFLAGS = -Wall -Wextra -O4 -std=gnu11

all:
	$(CC) $(CFLAGS) -o source source.c

build:
	docker build --tag pee:poo .

run:
	docker run pee:poo

clean: 
	rm -rf *.o source
