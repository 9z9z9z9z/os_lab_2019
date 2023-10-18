mutex :
	gcc mutex.c -o mutex -lpthread

locked_mutex :
	gcc mutex_locks.c -o locked_mutex -lpthread

deadlock : 
	gcc deadlock.c -o deadlock -lpthread

factorial :
	gcc factorial.c -o factorial -pthread

all : mutex locked_mutex factorial deadlock

clear :
	rm mutex locked_mutex factorial deadlock