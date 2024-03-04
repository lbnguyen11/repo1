#include <stdio.h>
#include <pthread.h>

//lock0
pthread_mutex_t lock0 = PTHREAD_MUTEX_INITIALIZER;

// //lock1
// typedef struct __lock_t { int flag; } lock_t;

// void init(lock_t *mutex) {
// 	// 0 -> lock is available, 1 -> held
// 	mutex->flag = 0;
// }

// // This is not a correct lock!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// void lock(lock_t *mutex) {
// 	while (mutex->flag == 1) // TEST the flag
// 	 ; // spin-wait (do nothing)
// 	mutex->flag = 1; // now SET it!
//  }

//  void unlock(lock_t *mutex) {
//  	mutex->flag = 0;
//  }

// lock_t lock1;

//lock2
int TestAndSet(int *old_ptr, int new) {
	int old = *old_ptr; // fetch old value at old_ptr
	*old_ptr = new; // store ’new’ into old_ptr
	return old; // return the old value
}

typedef struct __lock_t {
	int flag;
} lock_t;

void init(lock_t *lock) {
	// 0: lock is available, 1: lock is held
	lock->flag = 0;
}

void lock(lock_t *lock) {
	while (TestAndSet(&lock->flag, 1) == 1)
		; // spin-wait (do nothing)
}

void unlock(lock_t *lock) {
	lock->flag = 0;
}

lock_t lock2;


static volatile unsigned long counter = 0;
// mythread()
//
// Simply adds 1 to counter repeatedly
 void *mythread(void *arg) {
 	 //pthread_mutex_lock(&lock0);
 	 lock(&lock2);
	 printf("%d: begin\n", *(int *) arg);
	 unsigned long i;
	 for (i = 0; i < 100; i++) {
	 	counter = counter + 1;
	 	printf("(counter = %ld), *arg = %d, lock=%d\n", counter, *(int*)arg, lock2.flag);
	 }
	 printf("%d: done\n", *(int *) arg);
	 unlock(&lock2);
	 printf("DONE: (counter = %ld), *arg = %d, lock=%d\n", counter, *(int*)arg, lock2.flag);
	 //pthread_mutex_unlock(&lock0);
	 return NULL;
 }

 // main()
 //
 // Just launches threads (pthread_create)
 // and then waits for them (pthread_join)
 //
 int main(int argc, char *argv[]) {
 	 init(&lock2);
	 pthread_t ps[100];

	 //pthread_t p1, p2;
	 printf("main: begin (counter = %ld)\n", counter);
	 for (int i=0;i<5;i++)
	 {
	 	pthread_create(&ps[i], NULL, mythread, &ps[i]);
	 }
	 
	 // join waits for the threads to finish
	 for (int i=0;i<5;i++)
	 {
	 	pthread_join(ps[i], NULL);
	 }
	 printf("main: done with all (counter = %ld)\n", counter);
	 return 0;
 }
