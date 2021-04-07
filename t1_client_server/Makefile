CC=gcc

all: client.o server.o

client.o: client.c ./lib/request.o ./lib/request.h
	$(CC) -o client.o -D MEMORY=$(memory) -D NUM_SERVERS=$(n) client.c ./lib/request.o 

server.o: server.c queue.o queue.h ./logger/logger.o ./logger/logger.h
	$(CC) -o server.o -D MEMORY=$(memory) -D NUM_SERVERS=$(n) server.c queue.o ./logger/logger.o -pthread

logger.o: request.o request.h
	$(CC) -c -o logger.o ./logger/logger.c ../lib/request.o 

queue.o: queue.c
	$(CC) -c -o queue.o queue.c

request.o:
	$(CC) -c -o ./lib/request.c request.o

clean:
	rm client.o server.o queue.o
	rm -r server_logs
