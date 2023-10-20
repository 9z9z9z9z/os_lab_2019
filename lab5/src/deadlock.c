#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex3 = PTHREAD_MUTEX_INITIALIZER;

void *thread1() {
    pthread_mutex_lock(&mutex1);
    printf("Thread 1: started\n");
    sleep(1);
    pthread_mutex_lock(&mutex2);
    printf("Thread 1: ended\n");
    pthread_mutex_unlock(&mutex2);
    pthread_mutex_unlock(&mutex1);
    return NULL;
}

void *thread2() {
    pthread_mutex_lock(&mutex2);
    printf("Thread 2: started\n");
    sleep(1); 
    // pthread_mutex_lock(&mutex3);
    printf("Thread 2: ended\n");
    pthread_mutex_unlock(&mutex3);
    pthread_mutex_unlock(&mutex2);
    return NULL;
}

void *thread3() {
    pthread_mutex_lock(&mutex3);
    printf("Thread 3: started\n");
    sleep(1); 
    pthread_mutex_lock(&mutex1);
    printf("Thread 3: ended\n");
    pthread_mutex_unlock(&mutex1);
    pthread_mutex_unlock(&mutex3);
    return NULL;
}

int main() {
    pthread_t t1, t2, t3;
    
    if (pthread_create(&t1, NULL, thread1, NULL)) {
        perror("pthread_create");
        return 1;
    }

    if (pthread_create(&t2, NULL, thread2, NULL)) {
        perror("pthread_create");
        return 1;
    }
    if (pthread_create(&t3, NULL, thread3, NULL)) {
        perror("pthread_create");
        return 1;
    }

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    
    return 0;
}
