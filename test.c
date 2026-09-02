/*
CASE 0 : share memory
CASE 1 : test joint return
CASE 2 : detact
CASE 3 : thread id and deadlock
CASE 4 : test id
CASE 5 : test cancel thread
CASE 6 : race condition
CASE 7 : thread cleanup
CASE 8 : mutex
CASE 9 : trylock
CASE 10 : Read-Write Lock
CASE 11 : Thread-Specific Data
CASE 12 : Thread and signal
*/

#define CASE 8

#if CASE == 0
#include <stdio.h>
#include <pthread.h>

int *p1 = NULL;

void *worker1(void *arg)
{
    p1 = malloc(sizeof(int));
    *p1 = 30;

    sleep(5);

    printf("Thread 1 stopped!");

    return NULL;
}

void *worker2(void *arg)
{

    sleep(1);
    printf("%d\n", *p1);
    free(p1);
    printf("Thread 2 stopped!");

    return NULL;
}

int main(void)
{
    pthread_t id1, id2;

    pthread_create(&id1, NULL, worker1, NULL);
    pthread_create(&id2, NULL, worker2, NULL);

    pthread_join(id1, NULL);
    pthread_join(id2, NULL);
    return 0;
}
#elif CASE == 1
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/wait.h>

struct foo
{
    /* data */
    int a, b, c, d;
};

void *fn_thread1(void *arg)
{
    printf("Thread 1: start\n");

    int *rel = malloc(sizeof(int));

    *rel = 123;

    sleep(2);

    printf("Thread 1: end\n");

    return rel;
}

void *fn_thread2(void *arg)
{
    struct foo foo1 = {1, 2, 3, 4};
    // struct foo *foo1 = arg;

    printf("Struct in function thread 2:\n");
    printf(" structure at %p\n", &foo1);
    printf(" foo.a = %d\n", foo1.a);
    printf(" foo.b = %d\n", foo1.b);
    printf(" foo.c = %d\n", foo1.c);
    printf(" foo.d = %d\n", foo1.d);

    pthread_exit((void *)&foo1);
}

int main(void)
{
    pthread_t tid1, tid2;

    /* First thread */
    if (pthread_create(&tid1, NULL, fn_thread1, NULL) != 0)
    {
        printf("Create the first thread fail!\n");
        return 1;
    }

    printf("Main: waiting thread 1 ...\n");

    void *ret = NULL;

    /* join thread 1 */
    if (pthread_join(tid1, &ret) != 0)
    {
        printf("Error join thread 1!\n");
        return 3;
    }

    /* join thread 1 success */
    printf("Thread 1 finished with return value: ");
    printf("%d\n\n", *(int *)ret);

    free(ret);

    /* end first thread, start second thread */
    /* second thread */
    // struct foo foo1 = {1, 2, 3, 4};

    struct foo *fo = NULL;

    printf("Main prepare to create second thread\n");

    if (pthread_create(&tid2, NULL, fn_thread2, NULL) != 0)
    {
        printf("Create the second thread fail!\n");
        return 2;
    }

    /* join thread 2 */
    if (pthread_join(tid2, (void **)&fo) != 0)
    {
        printf("Error join thread 2!\n");
        return 4;
    }

    /* join thread 2 sucess */
    printf("Thread 2 finished with return value: ");
    printf("%d\n", *(int *)fo);

    printf("Struct in main:\n");
    printf("    structure at %p\n", fo);
    printf("    foo.a = %d\n", fo->a);
    printf("    foo.b = %d\n", fo->b);
    printf("    foo.c = %d\n", fo->c);
    printf("    foo.d = %d\n", fo->d);

    return 0;
}
#elif CASE == 2
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

void *delay_1(void *arg)
{
    printf("Thread is delaying 5s ...\n");
    sleep(5);
    printf("Thread stopped!\n");
    return NULL;
}

int main(void)
{
    int ret;

    pthread_t tid1;

    pthread_create(&tid1, NULL, delay_1, NULL);

    pthread_detach(tid1);

    printf("Main still running ...\n");

    ret = pthread_join(tid1, NULL);

    sleep(1);
    printf("%d (%s)\n", ret, strerror(ret));
    printf("Main stopped!\n");

    return 0;
}
#elif CASE == 3
#include <stdio.h>
#include <pthread.h>
#include <string.h>

pthread_t tid[3];

void *deadlock(void *arg)
{
    pthread_t my_id = pthread_self();

    if (pthread_equal(my_id, tid[0]))
    {
        printf("I am thread 1 and I am waitting for myself to exit!\n");

        int ret = pthread_join(my_id, NULL);

        printf("Thread 1 return value: %d (%s)\n", ret, strerror(ret));
    }
    else if (pthread_equal(my_id, tid[1]))
    {
        printf("I am thread 2!\n");
    }
    else if (pthread_equal(my_id, tid[2]))
    {
        printf("I am thread 3!\n");
    }
    return NULL;
}

void *deadlock1(void *arg)
{

    printf("I am thread 1 and I am waitting for thread 2 to exit!\n");

    int ret = pthread_join(tid[0], NULL);

    printf("Thread 1 join  return : %d (%s)\n", ret, strerror(ret));

    return NULL;
}

void *deadlock2(void *arg)
{

    printf("I am thread 2 and I am waitting for thread 1 to exit!\n");

    int ret = pthread_join(tid[0], NULL);

    printf("Thread 1 join  return : %d (%s)\n", ret, strerror(ret));

    return NULL;
}

int main(void)
{
    pthread_create(&tid[0], NULL, deadlock, NULL);
    // pthread_create(&tid[1], NULL, deadlock2, NULL);
    // pthread_create(&tid[2], NULL, worker, NULL);

    pthread_join(tid[0], NULL);
    // pthread_join(tid[1], NULL);
    // pthread_join(tid[2], NULL);

    return 0;
}
#elif CASE == 4
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

pthread_t ntid;

void print_ids(const char *s)
{
    pid_t pid;
    pthread_t tid;

    pid = getpid();
    tid = pthread_self();

    printf("%s pid: %lu tid: %lu (0x%lx)\n", s, (unsigned long)pid,
           (unsigned long)tid, (unsigned long)tid);
}

void *routine(void *arg)
{
    print_ids("new thread: ");

    // pthread_exit((void *)0);
    return ((void *)0);
}

int main(void)
{
    print_ids("main thread:");
    int err;

    err = pthread_create(&ntid, NULL, routine, NULL);

    if (err != 0)
        perror("can't create thread");

    sleep(1);

    exit(0);
}
#elif CASE == 5
#include <stdio.h>
#include <pthread.h>
#include <string.h>

pthread_t thread_id[2];

void *rountine(void *arg)
{
    if (pthread_equal(pthread_self(), thread_id[0]))
    {
        printf("Thread 1 will sleep in 5 seconds.\n");
        sleep(5);
    }
    else if (pthread_equal(pthread_self(), thread_id[1]))
    {
        printf("Thread 2 will cancel thread 1.\n");
        pthread_cancel(thread_id[0]);
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    for (int i = 0; i < 2; i++)
    {
        if (pthread_create(&thread_id[i], NULL, rountine, NULL) != 0)
        {
            printf("Create thread %d fail!\n", i + 1);
            return (i + 1);
        }
    }

    for (int i = 0; i < 2; i++)
    {
        int ret = pthread_join(thread_id[i], NULL);
        if (ret != 0)
        {
            printf("Create thread %d fail!\n", i + 1);
            return (i + 1);
        }
        else
        {
            printf("Thread %d exit with return value: %d (%s)\n",
                   i + 1, ret, strerror(ret));
        }
    }

    return 0;
}
#elif CASE == 6
#include <stdio.h>
#include <pthread.h>

void *rountine(void *arg)
{
    int *p = arg;
    /* add 1 to x and loop it 1000 times*/
    for (int i = 0; i < 1000; i++)
    {
        *p += 1;
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    int x = 0;
    /* i will create 1000 thread */
    int numthread = 1000;
    pthread_t thread_id[numthread];

    /* create thread */
    for (int i = 0; i < numthread; i++)
    {
        if (pthread_create(&thread_id[i], NULL, rountine, &x) != 0)
        {
            printf("Create thread %d fail.\n", i + 1);
            return (i + 1);
        }
    }

    /* join thread */
    for (int i = 0; i < numthread; i++)
    {
        if (pthread_join(thread_id[i], NULL) != 0)
        {
            printf("join thread %d fail.\n", i + 1);
            return (i + 101);
        }
    }

    /* ptint out the value of x after 1000 thread run */
    printf("x = %d\n", x);

    return 0;
}
#elif CASE == 7
#include <stdio.h>
#include <pthread.h>

void clean_up(void *arg)
{
    printf("Clean up: %s\n", (char *)arg);
}

void *routine1(void *arg)
{
    printf("Thread 1 strarted.\n");
    pthread_cleanup_push(clean_up, "thread 1 first handler");
    pthread_cleanup_push(clean_up, "thread 1 second handler");
    printf("thread 1 push complete\n");

    if (arg)
    {
        return ((void *)1);
    }

    pthread_cleanup_pop(0);
    pthread_cleanup_pop(0);

    return ((void *)1);
}

void *routine2(void *arg)
{
    printf("Thread 2 strarted.\n");
    pthread_cleanup_push(clean_up, "thread 2 first handler");
    pthread_cleanup_push(clean_up, "thread 2 second handler");
    printf("thread 2 push complete\n");

    if (arg)
    {
        pthread_exit((void *)2);
    }

    pthread_cleanup_pop(0);
    pthread_cleanup_pop(0);

    pthread_exit((void *)2);
}

int main(int argc, char *argv)
{
    pthread_t thid1, thid2;
    void *ret;

    if (pthread_create(&thid1, NULL, routine1, (void *)1) != 0)
    {
        printf("Create thread 1 fail.\n");
        return 1;
    }

    if (pthread_create(&thid2, NULL, routine2, (void *)1) != 0)
    {
        printf("Create thread 2 fail.\n");
        return 1;
    }

    if (pthread_join(thid1, &ret) != 0)
    {
        printf("Error join thread 1.\n");
        return 111;
    }

    printf("thread 1 exit with value: %d.\n", (int *)(ret));

    if (pthread_join(thid2, &ret) != 0)
    {
        printf("Error join thread 2.\n");
        return 222;
    }

    printf("thread 2 exit with value: %d.\n", (int *)(ret));

    return 0;
}
#elif CASE == 8
#include <stdio.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *rountine(void *arg)
{
    int *p = arg;
    /* add 1 to x and loop it 1000 times*/
    for (int i = 0; i < 1000; i++)
    {
        pthread_mutex_lock(&mutex);
        *p += 1;
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

void *rountine1(void *arg)
{
    int *p = arg;
    /* add 1 to x and loop it 1000 times*/
    for (int i = 0; i < 1000; i++)
    {
        *p += 1;
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    int x = 0;
    /* i will create 1000 thread */
    int numthread = 1000;
    pthread_t thread_id[numthread];
    pthread_t id;

    /* create thread */
    for (int i = 0; i < numthread; i++)
    {
        if (pthread_create(&thread_id[i], NULL, rountine, &x) != 0)
        {
            printf("Create thread %d fail.\n", i + 1);
            return (i + 1);
        }
    }

    /* create thread that doesn't use mutex lock */
    pthread_create(&id, NULL, rountine1, &x);

    /* join thread */
    for (int i = 0; i < numthread; i++)
    {
        if (pthread_join(thread_id[i], NULL) != 0)
        {
            printf("join thread %d fail.\n", i + 1);
            return (i + 101);
        }
    }

    /* join thread */
    pthread_join(id, NULL);

    /* ptint out the value of x after 1000 thread run */
    printf("x = %d\n", x);

    return 0;
}
#elif CASE == 9
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *routine1(void *arg)
{
    printf("Thread 1 will lock and sleep 5 seconds.\n");
    pthread_mutex_lock(&mutex);
    sleep(5);
    pthread_mutex_unlock(&mutex);
    printf("Thread 1 unlock.\n");
    return NULL;
}

void *routine2(void *arg)
{
    sleep(1);
    printf("Thread 2 try to lock during the thread 1 lock.\n");
    if (pthread_mutex_trylock(&mutex) == 0)
    {
    }
    else
    {
        printf("Thread 2 try to lock fail.\n");
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t thid1, thid2;

    pthread_create(&thid1, NULL, routine1, NULL);
    pthread_create(&thid2, NULL, routine2, NULL);

    pthread_join(thid1, NULL);
    pthread_join(thid2, NULL);

    return 0;
}
#elif CASE == 10
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;

int x = 0;

void *reader(void *arg)
{
    sleep(0.5);
    printf("Thread read x:\n");
    pthread_rwlock_rdlock(&rwlock);
    printf("%d\n", x);
    pthread_rwlock_unlock(&rwlock);

    return NULL;
}

void *writer(void *arg)
{
    printf("Thread will add 10 to x.\n");
    pthread_rwlock_wrlock(&rwlock);
    x += 10;
    pthread_rwlock_unlock(&rwlock);
    printf("Thread write unlock.\n");
    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t thid1[10000], thid2;

    /* create 10000 thread reader  */
    for (int i = 0; i < 10000; i++)
    {
        if (pthread_create(&thid1[i], NULL, reader, NULL) != 0)
        {
            printf("Create thread %d fail.\n", i + 1);
            return (i + 1);
        }
    }
    /* create thread writer */
    pthread_create(&thid2, NULL, writer, NULL);

    /* join thread reader */
    for (int i = 0; i < 10000; i++)
    {
        if (pthread_join(thid1[i], NULL) != 0)
        {
            printf("Join thread %d fail.\n", i + 1);
            return (i + 1);
        }
    }

    /* join thread writer */
    pthread_join(thid2, NULL);

    return 0;
}
#elif CASE == 11
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

pthread_key_t buffer_key;

struct data
{
    int number;
    char text[20];
    bool state;
};


void destructor(void *ptr)
{
    printf("Free buffer %p\n", ptr);
    free(ptr);
}

void process_data(const char *data)
{
    struct data *buffer = pthread_getspecific(buffer_key);

    snprintf(buffer->text, sizeof(buffer->text), "%s", data);

    printf("Thread %lu: buffer = %p, content = %s\n",
           pthread_self(), (void *)buffer, buffer->text);
}

void log_data(void)
{
    struct data *buffer = pthread_getspecific(buffer_key);

    printf("Thread %lu: buffer = %p, content = %s\n",
           pthread_self(), (void *)buffer, buffer->text);
}

void *routine(void *arg)
{
    struct data *buffer = malloc(sizeof(struct data));

    pthread_setspecific(buffer_key, buffer);

    printf("Thread %lu: set buffer = %p\n", pthread_self(), (void *)buffer);

    if (*(int *)arg == 1)
    {
        process_data("Hello1");
        sleep(1);
        log_data();
    }
    else
    {
        process_data("Hello2");
    }

    log_data();

    return NULL;
}

int main(void)
{
    pthread_t t1, t2;
    int id[2] = {1, 2};

    pthread_key_create(&buffer_key, destructor);

    pthread_create(&t1, NULL, routine, &id[0]);
    pthread_create(&t2, NULL, routine, &id[1]);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_key_delete(buffer_key);

    return 0;
}
#elif CASE == 12
#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

void signal_handler(int sig)
{
    printf("Thread %lu received signal %d\n",
           pthread_self(), sig);
}

void *routine(void *arg)
{
    int id = *(int *)arg;

    printf("Thread %d is running.\n", id);

    while(1)
    {
        sleep(1);
    }

    printf("Thread %d is stopped.\n", id);
    return NULL;
}

int main(int argc, char *argv[])
{
    int numthread = 5;
    int id[] = {1, 2, 3, 4, 5};
    pthread_t thread_id[numthread];

    /* create signal */
    signal(SIGTERM, signal_handler);

    /* create thread */
    for (int i = 0; i < numthread; i++)
    {
        if (pthread_create(&thread_id[i], NULL, routine, &id[i]) != 0)
        {
            printf("Create thread %d fail.\n", i + 1);
            return (i + 1);
        }
    }

    sleep(5);

    /* send signal to thread */
    pthread_kill(thread_id[2], SIGTERM);

    /* join thread */
    for (int i = 0; i < numthread; i++)
    {
        if (pthread_join(thread_id[i], NULL) != 0)
        {
            printf("join thread %d fail.\n", i + 1);
            return (i + 101);
        }
    }

    return 0;
}
#endif