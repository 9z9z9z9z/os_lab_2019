CC = gcc
CFLAGS = -I.

parallel_min_max : utils.o find_min_max.o
	$(CC) -o parallel_min_max utils.o find_min_max.o parallel_min_max.c $(CFLAGS)

process_memory :
	$(CC) -o process_memory process_memory.c $(CFLAGS)

parallel_sum : utils.o
	$(CC) -o parallel_sum utils.o parallel_sum.c -lpthread

utils.o : utils.h
	$(CC) -o utils.o -c utils.c $(CFLAGS)

debug_parallel_sum : utils.o
	$(CC) -g -o debug_parallel_sum utils.o parallel_sum.c -lpthread

zombie : 
	${CC} -o  zombie zombie.c $(CFLAGS)

zombie_correct : 
	${CC} -o zombie_correct zombie_correct.c $(CFLAGS)

clear : 
	rm utils.o find_min_max.o parallel_min_max zombie_correct zombie process_memory parallel_sum