all: main clean

main: main.o client.o config.o
	gcc -o out/main main.o client.o config.o -ljansson -lulfius -lconfig

main.o:	src/main.c
	gcc -c src/main.c

client.o: src/client/client.c src/client/client.h
	gcc -c src/client/client.c

config.o: src/utils/config.c src/utils/config.h
	gcc -c src/utils/config.c

install:
	sudo apt-get install libconfig-dev
	sudo apt install libmicrohttpd-dev libjansson-dev libcurl4-gnutls-dev libgnutls28-dev libgcrypt20-dev libsystemd-dev

clean:
	rm *.o