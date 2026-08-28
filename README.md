# Thread

![alt text](image.png)

> 'A thread is a single sequence stream within a process and is called a lightweight process because it is smaller and faster. It allows multiple tasks to run simultaneously, improving program efficiency.'

![alt text](image-1.png)

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

Threads are needed in modern operating systems and applications because they:

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

# Threading Issues

- **fork() and exec()**: In multithreaded programs, fork() may duplicate all threads or just the calling thread, depending on the system. exec() replaces the entire process including all threads with the new program.
- **Signal Handling**: Signals notify a process of events. They can be synchronous or asynchronous and are handled by either the default kernel handler or a user-defined handler.
- **Thread Cancellation**: Threads can be terminated before completion. Cancellation can be asynchronous (immediate) or deferred (thread checks periodically). Example: stopping all threads loading a webpage.
- **Thread-Local Storage (TLS)**: Threads share process data, but sometimes need private copies, such as unique identifiers for transactions.
- **Scheduler Activations**: The kernel provides virtual processors, allowing a user-thread library to schedule threads efficiently.