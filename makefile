notDefault:
	@echo "No arguments runs the first recipe no matter what you call it."
compile: client server
client: client.o pipe.o
	gcc -o client client.o pipe.o 
server: server.o pipe.o 
	gcc -o server server.o pipe.o
client.o: client.c pipe.h
	gcc -c client.c
server.o: server.c pipe.h
	gcc -c server.c 
pipe.o: pipe.c pipe.h
	gcc -c pipe.c
clean:
	rm *.o
	rm *~
