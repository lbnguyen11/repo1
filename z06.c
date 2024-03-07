#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 2
#define TRUE 1
#define FALSE 0

unsigned long counter = 0;

volatile int turn;
volatile int flag[NUM_THREADS] = {FALSE, FALSE}; // Flags for 2 threads

void *peterson(void *arg) {
    int id = *((int *)arg);
    flag[id] = TRUE;
    turn = 1 - id;
    while (flag[1 - id] && turn == 1 - id) {}
    // Critical Section
    printf("Thread %d is in critical section\n", id);
    int c=0;
    for (;c<2000000000;c++)
    {
    	counter = counter + 1;
    }
    printf("Thread %d increases counter to %ld,i=%d\n", id,counter,c);
    // Remainder Section
    flag[id] = FALSE;
    pthread_exit(NULL);
}

int main() {
    pthread_t tid[NUM_THREADS];
    int i, ids[NUM_THREADS];

    // Create threads with unique IDs
    for (i = 0; i < NUM_THREADS; i++) {
        ids[i] = i;
        pthread_create(&tid[i], NULL, peterson, &ids[i]);
    }

    // Join threads
    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(tid[i], NULL);
    }

    return 0;
}
