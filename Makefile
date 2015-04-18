all: webserver

webserver: webserver.c
	gcc -O3 -Wall webserver.c -o webserver.out

debug: webserver.c
	gcc -DDEBUG -Wall webserver.c -o webserver.out

clean:
	rm webserver.out