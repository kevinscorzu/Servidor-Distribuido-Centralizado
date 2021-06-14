all: main clean

semaphorefix: fix clean 

main: main.o node.o image.o config.o semaphore.o
	gcc -o out/main main.o node.o image.o config.o semaphore.o -ljansson -lulfius -lconfig -lpthread

fix: clean.o semaphore.o
	gcc -o out/clean clean.o semaphore.o -lpthread

clean.o:
	gcc -c src/clean.c

main.o:	src/main.c
	gcc -c src/main.c

node.o: src/node/node.c src/node/node.h
	gcc -c src/node/node.c

image.o: src/node/image.c src/node/image.h
	gcc -c src/node/image.c

config.o: src/utils/config.c src/utils/config.h
	gcc -c src/utils/config.c

semaphore.o: src/utils/semaphore.c src/utils/semaphore.h
	gcc -c src/utils/semaphore.c

install:
	sudo apt-get install libconfig-dev
	sudo apt install libmicrohttpd-dev libjansson-dev libcurl4-gnutls-dev libgnutls28-dev libgcrypt20-dev libsystemd-dev

clean:
	rm *.o