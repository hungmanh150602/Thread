# Thread

![alt text](image.png)

> 'A thread is a single sequence stream within a process and is called a lightweight process because it is smaller and faster. It allows multiple tasks to run simultaneously, improving program efficiency.'

![alt text](image-1.png)

## Why we need thread?

When we create a new **process**:

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

**Thread** are needed in modern operating systems and applications because they:

- **Improve Performance**: Threads allow multiple tasks to run at the same time (parallel or interleaved), making programs execute faster.
- **Increase Responsiveness**: If one thread is busy, another can handle user actions, keeping the application responsive.
- **Enable Concurrency**: Multiple operations like saving files, processing data, and handling user input can happen simultaneously.
- **Better CPU Utilization**: On multi-core systems, threads can run on different cores, improving overall system performance.
- **Efficient Resource Sharing**: Threads share the same memory and resources within a process, making communication faster and reducing overhead.

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

# Components of Threads

These are the basic components of the Operating System.

- **Stack Space**: Stores local variables, function calls, and return addresses specific to the thread.
- **Register Set**: Hold temporary data and intermediate results for the thread's execution.
- **Program Counter**: Tracks the current instruction being executed by the thread.

# Types of Threads

Threads are mainly classified based on how they are managed and scheduled in an operating system. There are two primary types of threads.

- User Level Thread
- Kernel Level Thread

![alt text](image-2.png)

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

**Thread** has a weakness:  
If thread 1 and thread 2 access simultaneously into an address, it is a **race condition**.  
Therefore, it needs:

```text
mutex
condition variable
semaphore
```

## Thread ID

> Thread ID is used to distinguish between threads.

- **Manage threads**: When you want to control a specific thread (for example: to pause or cancel it, or to wait for it to finish using `thread_joint(id, NULL)`), you must specify it's ID.
- **Monitoring and Debuging**: when printing messages to the screen (logging), including thread ID helps you identify exactly which thread is executing.

Prototype:
```c
extern pthread_t pthread_self (void);
````

An examle of printing the thread ID:

```c
void* print_id(void* arg) {
    pthread_t my_id = pthread_self();
    
    printf("Worker thread dang chay. ID cua toi la: %lu\n", (unsigned long)my_id);
    
    return NULL;
}

int main() {
    pthread_t thread1, thread2;

    pthread_create(&thread1, NULL, print_id, NULL);
    pthread_create(&thread2, NULL, print_id, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    return 0;
}
```

```text
Thread id: 131767210079808
Thread id: 131767201687104
```

# pthread_join()

> The current thread waits for a specific thread to finish.

Prototype:

```c
int pthread_join(
    pthread_t thread,   /* thread id want to wait */
    void **return_value /* the value of thread return */
);
```

Return value:

> On success, `pthread_join()` returns 0; on error, it returns an error number.

Example:

```c
void *worker(void *arg)
{
    printf("Worker: start\n");

    int *rel = malloc(sizeof(int));

    *rel = 123;

    sleep(2);

    printf("Worker: end\n");

    return rel;
}

int main(void)
{
    pthread_t tid;

    void *ret = NULL;

    pthread_create(&tid, NULL, worker, NULL);

    printf("Main: waiting...\n");

    pthread_join(tid, &ret);

    printf("Main: worker finished\n");

    printf("%d\n", *(int *)ret);
    free(ret);

    return 0;
}
```

```text
Main: waiting...
Worker: start
Worker: end
Main: worker finished
123                      <-------- the return value of thread
```

Timeline:

```text
                         PROCESS
┌──────────────────────────────────────────────────────────────┐
│                                                              │
│ MAIN THREAD                         WORKER THREAD            │
│                                                              │
│ pthread_create() ──────────────────────► worker()            │
│      │                                    │                  │
│      │                                    ├─ printf start    │
│      │                                    │                  │
│      │                                    ├─ malloc()        │
│      │                                    │       │          │
│      │                                    │       ▼          │
│      │                                    │    HEAP          │
│      │                                    │   [   ?   ]      │
│      │                                     │       │         │
│      │                                    ├─ *rel = 123      │
│      │                                    │       │          │
│      │                                    │       ▼          │
│      │                                    │    [ 123 ]       │
│      ▼                                    │                  |
│ printf waiting                           ├─ sleep(2)         │
│      │                                   │                   │
│      ▼                                   │                   │
│ pthread_join()                           │                   │
│      │                                   │                   │
│      │ WAIT                              │                   │
│      │                                   ├─ printf end       │
│      │                                   │                   │
│      │                                   ├─ return rel       │
│      │                                   │       │           │
│      │                                   │       ▼           │
│      │                                   │   return 0x5000   │
│      │                                   │                   │
│      ◄───────────────────────────────────┘                   │
│      │                                                       │
│      ▼                                                       │
│ ret = 0x5000                                                 │
│      │                                                       │
│      ▼                                                       │
│ *(int *)ret → 123                                            │
│      │                                                       │
│      ▼                                                       │
│ free(ret)                                                    │
│      │                                                       │
│      ▼                                                       │
│  HEAP MEMORY RELEASED                                        │
│                                                              │
└──────────────────────────────────────────────────────────────┘
```

Without `pthread_join()`, the `main` function might terminate before the thread completes its work.

# Terminate Thread

- calls `pthread_exit()`
- returns from `start_routine()`

# Threading Issues

- **fork() and exec()**: In multithreaded programs, fork() may duplicate all threads or just the calling thread, depending on the system. exec() replaces the entire process including all threads with the new program.
- **Signal Handling**: Signals notify a process of events. They can be synchronous or asynchronous and are handled by either the default kernel handler or a user-defined handler.
- **Thread Cancellation**: Threads can be terminated before completion. Cancellation can be asynchronous (immediate) or deferred (thread checks periodically). Example: stopping all threads loading a webpage.
- **Thread-Local Storage (TLS)**: Threads share process data, but sometimes need private copies, such as unique identifiers for transactions.
- **Scheduler Activations**: The kernel provides virtual processors, allowing a user-thread library to schedule threads efficiently.