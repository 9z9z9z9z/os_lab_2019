PORT_TCP = 10050
PORT_UDP = 20001
BUFSIZE = 100

stopTCP :
	fuser -k $(PORT_TCP)/tcp 

stopUDP :
	fuser -k serverUDP

clientTCP :
	gcc tcpclient.c -g -o clientTCP -lpthread

serverTCP :
	gcc tcpserver.c -o serverTCP -lpthread

tcp : clientTCP serverTCP

serverUDP :
	gcc udpserver.c -o serverUDP -lpthread

clientUDP :
	gcc udpclient.c -o clientUDP -lpthread

udp : clientUDP serverUDP

run_tcp :
	./serverTCP --SERV_PORT $(PORT_TCP) --BUFSIZE $(BUFSIZE) &

debug : 
	gdb ./clientTCP

run_client_tcp : 
	./clientTCP --SERV_PORT $(PORT_TCP) --BUFSIZE $(BUFSIZE) 

run_udp :
	./serverUDP --SERV_PORT $(PORT_UDP) --BUFSIZE $(BUFSIZE) &

all : udp tcp

clear :
	rm clientTCP clientUDP serverTCP serverUDP
