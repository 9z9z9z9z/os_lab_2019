#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void do_one_thing(int *);
void do_another_thing(int *);
void do_wrap_up(int);
void lock_mutex(pthread_mutex_t*);
void unlock_mutex(pthread_mutex_t*);
int common = 0;
int r1 = 0, r2 = 0, r3 = 0;
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

int main() {
  pthread_t thread1, thread2;

  if (pthread_create(&thread1, NULL, (void *)do_one_thing,
			  (void *)&common) != 0) {
    perror("pthread_create");
    exit(1);
  }

  if (pthread_create(&thread2, NULL, (void *)do_another_thing,
                     (void *)&common) != 0) {
    perror("pthread_create");
    exit(1);
  }

  if (pthread_join(thread1, NULL) != 0) {
    perror("pthread_join");
    exit(1);
  }

  if (pthread_join(thread2, NULL) != 0) {
    perror("pthread_join");
    exit(1);
  }

  do_wrap_up(common);

  return 0;
}

void lock_mutex(pthread_mutex_t* mut) {
    asm volatile(
        "lock:\n"
        "    xorl %%eax, %%eax\n"
        "    incl %%eax\n"
        "    lock xchg %%eax, %0\n"
        "    test %%eax, %%eax\n"
        "    jnz lock"
        : "+m" (*mut)
        :
        : "%eax"
    );
}

void unlock_mutex(pthread_mutex_t* mut) {
    asm volatile(
        "movl $0, %0"
        : "+m" (*mut)
        :
    );
}

void do_one_thing(int *pnum_times) {
  int i, j, x;
  unsigned long k;
  int work;
  for (i = 0; i < 50; i++) {
    lock_mutex(&mut);
    printf("doing one thing\n");
    work = *pnum_times;
    printf("counter = %d\n", work);
    work++;
    for (k = 0; k < 500000; k++);
    *pnum_times = work;
    unlock_mutex(&mut);
  }
}

void do_another_thing(int *pnum_times) {
  int i, j, x;
  unsigned long k;
  int work;
  for (i = 0; i < 50; i++) {
    lock_mutex(&mut);
    printf("doing another thing\n");
    work = *pnum_times;
    printf("counter = %d\n", work);
    work++;
    for (k = 0; k < 500000; k++);
    *pnum_times = work;
    unlock_mutex(&mut);
  }
}

void do_wrap_up(int counter) {
  int total;
  printf("All done, counter = %d\n", counter);
}