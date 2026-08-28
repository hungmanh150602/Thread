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
    printf("Thread 1 is preparing add 10 to x = %d...\n", *p);
    sleep(0.5);
    *p = *p + 10;
    printf("Thread 1 after add: x = %d\n", *p);
    return NULL;
}

void *sub_10(void *arg)
{
    int *p = arg;
    printf("Thread 2 subtract x = %d", *p);
    *p -= 10;
    printf(" by 10, then x = %d\n", *p);
    return NULL;
}

void *print_id(void *arg)
{
    pthread_t id = pthread_self();

    printf("Thread id: %lu\n", id);

    return NULL;
}

void *test_return(void *arg)
{
    int *rel = malloc(sizeof(int));

    *rel = 123;

    return rel;
}

int main(void)
{
    int x = 100;

    pthread_t tid1;
    pthread_t tid2;

    pthread_create(&tid1,    /*  consit thread id */
                   NULL,     /* config of thread */
                   print_id, /* function pointer */
                   NULL);    /* argument pass into function pointer */

    void *ret = NULL;

    pthread_create(&tid2,    /*  consit thread id */
                   NULL,     /* config of thread */
                   test_return, /* function pointer */
                   NULL);    /* argument pass into function pointer */

    // pthread_join(tid, NULL);
    // sleep(1);
    // printf("Finally, x = %d\n", x);

    // pid_t pid = fork();

    // if (pid == 0)
    // {
    //     exit(10);
    // }

    pthread_join(tid1, NULL);
    pthread_join(tid2, &ret);

    printf("%d\n", *(int *)ret);
    free(ret);
    return 0;
}