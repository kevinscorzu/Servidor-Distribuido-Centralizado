all: main clean

semaphorefix: fix clean 

main: main.o server.o connect.o config.o log.o queue.o semaphore.o
	gcc -o out/main main.o server.o connect.o config.o log.o queue.o semaphore.o -ljansson -lulfius -lconfig -lpthread

fix: clean.o log.o semaphore.o
	gcc -o out/clean clean.o log.o semaphore.o -lpthread

clean.o:
	gcc -c src/clean.c

main.o:	src/main.c
	gcc -c src/main.c

server.o: src/server/server.c src/server/server.h
	gcc -c src/server/server.c

connect.o: src/server/connect.c src/server/connect.h
	gcc -c src/server/connect.c

config.o: src/utils/config.c src/utils/config.h
	gcc -c src/utils/config.c

log.o: src/utils/log.c src/utils/log.h
	gcc -c src/utils/log.c

queue.o: src/utils/queue.c src/utils/queue.h
	gcc -c src/utils/queue.c

semaphore.o: src/utils/semaphore.c src/utils/semaphore.h
	gcc -c src/utils/semaphore.c

install:
	sudo apt-get install libconfig-dev
	sudo apt install libmicrohttpd-dev libjansson-dev libcurl4-gnutls-dev libgnutls28-dev libgcrypt20-dev libsystemd-dev

clean:
	rm *.o