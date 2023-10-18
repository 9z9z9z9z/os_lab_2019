client :
	gcc client.c -o client

server:
	gcc server.c -o server -lpthread

run_servers :
	./server --port 20001 --tnum 4 &
	./server --port 20002 --tnum 4 &

stop :
	fuser -k 20001/tcp && fuser -k 20002/tcp

all : client server

clear :
	rm client server