// build with -lpthread and run with sudo for setting sched priority
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

pthread_spinlock_t lock;
unsigned long counter = 0;

void *increment_counter(void *thread_id) {
    long tid = (long)thread_id;

    // Acquire the spin lock
    pthread_spin_lock(&lock);

    // Critical section
    for (unsigned long i=0;i<1000000000;i++)
    {
        counter = counter + 1;
        printf("Thread %ld incremented counter to %ld\n", tid, counter);
    }
    

    // Release the spin lock
    pthread_spin_unlock(&lock);

    pthread_exit(NULL);
}

int main() {
    const int NUM_THREADS = 100;
    pthread_t threads[NUM_THREADS];
    int i;

    int s;

    /////////////////////////////////////////////////////////////////
    pthread_attr_t attr0;
    struct sched_param param0;

    // Initialize thread attributes
    pthread_attr_init(&attr0);

    // Set thread to inherit scheduling attributes
    pthread_attr_setinheritsched(&attr0, PTHREAD_EXPLICIT_SCHED);

    // Set scheduling policy to FIFO
    pthread_attr_setschedpolicy(&attr0, SCHED_RR);

    // Set thread priority
    param0.sched_priority = sched_get_priority_min(SCHED_RR);
    pthread_attr_setschedparam(&attr0, &param0);


    ///////////////////////////////////////////////////////////////////
    pthread_attr_t attr;
    struct sched_param param;

    // Initialize thread attributes
    pthread_attr_init(&attr);

    // Set thread to inherit scheduling attributes
    pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);

    // Set scheduling policy to FIFO
    pthread_attr_setschedpolicy(&attr, SCHED_RR);

    // Set thread priority
    param.sched_priority = sched_get_priority_max(SCHED_RR);
    pthread_attr_setschedparam(&attr, &param);


    // Initialize spin lock
    if (pthread_spin_init(&lock, 1) != 0) {
        printf("Spin lock initialization failed\n");
        return -1;
    }

    pthread_create(&threads[0], &attr0, increment_counter, (void *)0);
    usleep(1);

    // Create threads
    for (i = 1; i < NUM_THREADS; i++) {
        s = pthread_create(&threads[i], &attr, increment_counter, (void *)i);
        if (s != 0) {
            printf("Thread creation failed, error=%d\n",s);
            return -1;
        }
    }

    // Join threads
    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Destroy spin lock
    pthread_spin_destroy(&lock);

    return 0;
}

