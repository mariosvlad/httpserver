# httpserver
Http (version 1.1) server with thread pool (educational project)
Implemented requests: GET, HEAD, DELETE

You will find the program in "web" folder. 
The server is able to serve concurrently one specific number of requests. Since every thread serves one request each time, this number is the number of available threads in thread pool. If  the server receives more requests than this number, then they ignored.

POST, PUT not implemented
