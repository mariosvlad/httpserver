all: webserver

webserver: webserverThreads.c
	gcc -O3 -pthread -Wall webserverThreads.c -o webserver.out

debug: webserver.c
	gcc -DDEBUG -pthread -Wall webserverThreads.c -o webserver.out

clean:
	rm webserver.out