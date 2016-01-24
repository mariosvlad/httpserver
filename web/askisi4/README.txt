Instructions to execute the program:

1. Compile the program typing in the command line of terminal the following:
gcc -pthread -o http webserverThreads.c
The executable file "http" has been created.

2. Then you can use the telnet command, the browser or any special tool to send requests to the server HEAD, GET, DELETE using the address:
http://localhost:[port in config.txt]/[file]
for example: http://localhost:30000/supply/contact.html

You can see the current state of the program in terminal.
