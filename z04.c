Certainly! Here's a simple example demonstrating a lock-free counter using the compare-and-swap (CAS) operation in C:

```c
#include <stdio.h>
#include <stdatomic.h>
#include <pthread.h>

// Global atomic counter
atomic_int counter = ATOMIC_VAR_INIT(0);

void* increment_counter(void* arg) {
    for (int i = 0; i < 1000000; ++i) {
        int current_value, new_value;
        do {
            current_value = atomic_load(&counter); // Load the current value of the counter
            new_value = current_value + 1; // Calculate the new value
        } while (!atomic_compare_exchange_weak(&counter, &current_value, new_value)); // Perform CAS operation
    }
    return NULL;
}

int main() {
    pthread_t threads[5];

    // Create threads
    for (int i = 0; i < 5; ++i) {
        pthread_create(&threads[i], NULL, increment_counter, NULL);
    }

    // Join threads
    for (int i = 0; i < 5; ++i) {
        pthread_join(threads[i], NULL);
    }

    // Print the final value of the counter
    printf("Final counter value: %d\n", atomic_load(&counter));

    return 0;
}
```

In this example:

- We use `atomic_int` to declare an atomic integer counter.
- Each thread increments the counter using a loop with a CAS operation.
- Inside the loop, the thread first loads the current value of the counter.
- Then, it calculates the new value by incrementing the current value.
- Finally, it attempts to perform a CAS operation using `atomic_compare_exchange_weak`. This operation compares the current value of the counter with the value loaded earlier. If they match, it updates the counter's value to the new value. If the CAS operation fails (indicating that another thread modified the counter concurrently), the thread retries the operation until it succeeds.
- After all threads finish incrementing the counter, the main function prints the final value of the counter.

This example demonstrates how to implement a lock-free counter using the CAS operation in C.