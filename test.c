#include <stdio.h>
#include <stddef.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

char *str = "hehe";

int *p1 = NULL;

void *worker1(void *arg)
{
    p1 = malloc(sizeof(int));
    *p1 = 30;
    int *p = arg;

    (*p)++;

    printf("%s\n", str);
    printf("Hello from thread %d\n \
        Then I sleep 5 seconds!\n",
           *p);

    sleep(5);
    printf("%s\n", str);

    return NULL;

    while (1)
    {
        printf("hello\n");
        sleep(1);
    }
}

void *worker2(void *arg)
{

    sleep(1);
    printf("%d\n", *p1);
    free(p1);
    return NULL;
}

void *add_10(void *x)
{
    int *p = x;
    *p = *p + 10;
    printf("Thread 1: %d\n", *p);
    return NULL;
}

void *sub_10(void *arg)
{
    int *p = arg;
    *p -= 10;
    printf("Thread 2: %d\n", *p);
    return NULL;
}

int main(void)
{
    int x = 100;

    pthread_t tid1;
    pthread_t tid2;

    pthread_create(&tid1,  /*  consit thread id */
                   NULL,   /* config of thread */
                   add_10, /* function pointer */
                   &x);    /* argument pass into function pointer */

    pthread_create(&tid2,  /*  consit thread id */
                   NULL,   /* config of thread */
                   sub_10, /* function pointer */
                   &x);    /* argument pass into function pointer */

    // pthread_join(tid, NULL);
    sleep(1);
    printf("%d\n", x);

    // pid_t pid = fork();

    // if (pid == 0)
    // {
    //     exit(10);
    // }

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    return 0;
}