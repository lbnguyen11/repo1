#include <stdio.h>
#include <pthread.h>

//pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

typedef struct __lock_t { int flag; } lock_t;

void init(lock_t *mutex) {
	// 0 -> lock is available, 1 -> held
	mutex->flag = 0;
}

// This is not a correct lock!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void lock(lock_t *mutex) {
	while (mutex->flag == 1) // TEST the flag
	 ; // spin-wait (do nothing)
	mutex->flag = 1; // now SET it!
 }

 void unlock(lock_t *mutex) {
 	mutex->flag = 0;
 }

lock_t lock1;

static volatile unsigned long counter = 0;
// mythread()
//
// Simply adds 1 to counter repeatedly
 void *mythread(void *arg) {
 	 //pthread_mutex_lock(&lock);
 	 lock(&lock1);
	 printf("%s: begin\n", (char *) arg);
	 unsigned long i;
	 for (i = 0; i < 100; i++) {
	 	counter = counter + 1;
	 }
	 printf("%s: done\n", (char *) arg);
	 unlock(&lock1);
	 //pthread_mutex_unlock(&lock);
	 return NULL;
 }

 // main()
 //
 // Just launches threads (pthread_create)
 // and then waits for them (pthread_join)
 //
 int main(int argc, char *argv[]) {
 	 init(&lock1);
	 pthread_t ps[100000];

	 //pthread_t p1, p2;
	 printf("main: begin (counter = %ld)\n", counter);
	 for (int i=0;i<1000;i++)
	 {
	 	pthread_create(&ps[i], NULL, mythread, "myname");
	 }
	 
	 // join waits for the threads to finish
	 for (int i=0;i<1000;i++)
	 {
	 	pthread_join(ps[i], NULL);
	 }
	 printf("main: done with all (counter = %ld)\n",
	 counter);
	 return 0;
 }