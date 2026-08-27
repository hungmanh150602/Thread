# Thread

![alt text](image.png)

## Why we need thread?

When we create a new process:

```text
Process A
┌──────────────────────────────┐
│ Address Space                │
│                              │
│ Code                         │
│ Data                         │
│ Heap                         │
│ Stack                        │
│                              │
│ File descriptors             │
└──────────────────────────────┘
             │
           fork()
             │
             ▼
Process B
┌──────────────────────────────┐
│ Address Space                │
│ Code                         │
│ Data                         │
│ Heap                         │
│ Stack                        │
│                              │
│ File descriptors             │
└──────────────────────────────┘
```

Each process have specific address space.  
If you want to communication A and B, you need IPC:

- pipe
- share memory
- socket
- message queue
- . . .

**Thread** is different.  
Thread inside process

```text
                 Process
┌─────────────────────────────────────┐
│                                     │
│   Shared Address Space              │
│                                     │
│   Code                              │
│   Global data                       │
│   Heap                              │
│                                     │
│      ┌─────────────┐                │
│      │ Main Thread │                │
│      │ Stack       │                │
│      └─────────────┘                │
│                                     │
│      ┌─────────────┐                │
│      │ Worker      │                │
│      │ Thread      │                │
│      │ Stack       │                │
│      └─────────────┘                │
│                                     │
└─────────────────────────────────────┘
```

Threads share:

```text
Code
Global variables
Heap
Open file descriptors
...
```

but each thread has:

```c
Register state
Stack
Thread ID
Scheduling state
```

## Compare Thread and Proces

**Process**

|Strength|Weakness|
|:-----|:-----|
|Good isolation|Creation/management is usually more resource-intensive.|
|Good fail isolaton|Complex IPC|
|Good security boundary|Data sharing is more difficult.|
|Specific address space||

**Thread**

|Strength|Weakness|
|:-----|:-----|
|Consumes less memory than the process|Shared memory → race condition|
|Easily share memories|Need synchronization|
|Inter-thread communication is very fast|A memory error can affect the entire process.|
|Suitable for parallel/concurrent work within the same application|A deadlock can occur.|

# Library

POSIX threads library (libpthread, -lpthread)

```c
#include <ptheread.h>
```

# Create thread

1. Prototype

```c
extern int pthread_create (pthread_t *__restrict __newthread,
      const pthread_attr_t *__restrict __attr,
      void *(*__start_routine) (void *),
      void *__restrict __arg) __THROWNL __nonnull ((1, 3));
```

1. Example

```c
void *worker(void *arg)
{
    int *p = arg;

    (*p)++;

    printf("Hello from thread %d\n", *p);
    return NULL;
}

int main(void)
{
    int x = 100;

    pthread_t tid;

    pthread_create(&tid,   /*  consit thread id */
                   NULL,   /* config of thread */
                   worker, /* function pointer */
                   &x);    /* argument pass into function pointer */

    pthread_join(tid, NULL);
    return 0;
}
```

1. After create thread we have:

```text
                Process
                   │
        ┌──────────┴──────────┐
        │                     │
   Main Thread            Worker Thread
        │                     │
     main()                worker()
        │                     │
        │                  arg = &x
        │                     │
        └──────────┬──────────┘
                   │
              shared memory
```

Thread has a weakness:  
If thread 1 and thread 2 access simultaneously into an address, it is a **race condition**.  
Because it need:

```text
mutex
condition variable
semaphore
```

# Terminate Thread

- calls `pthread_exit()`
- returns from `start_routine()`