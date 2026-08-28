/*
CASE 0 : share memory
CASE 1 : test return
CASE 2 : detact
CASE 3 : thread id and deadlock
*/

#define CASE 3

#if CASE == 0
#include <stdio.h>
#include <pthread.h>

void *worker1(void *arg)
{
    p1 = malloc(sizeof(int));
    *p1 = 30;
    int *p = arg;

    (*p)++;

    printf("Hello from thread %d\n \
        Then I sleep 5 seconds!\n",
           *p);

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
#elif CASE == 2
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

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
    printf("%d\n", ret);
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

    if(pthread_equal(my_id, tid[0]))
    {
        printf("I am thread 1 and I am waitting for myself to exit!\n");

        int ret = pthread_join(my_id, NULL);
        
        printf("Thread 1 return value: %d (%s)\n", ret, strerror(ret));
    }
    else if(pthread_equal(my_id, tid[1]))
    {
        printf("I am thread 2!\n");
    }
    else if(pthread_equal(my_id, tid[2]))
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
#endif