#include <stdio.h>
#include <stddef.h>
#include <pthread.h>
#include <unistd.h>

char *str = "hehe";

void *worker(void *arg)
{
    int *p = arg;

    (*p)++;

    printf("%s\n", str);
    printf("Hello from thread %d\n \
        Then I sleep 5 seconds!\n",
           *p);

    sleep(5);

    return NULL;

    while (1)
    {
        printf("hello\n");
        sleep(1);
    }
    
}

int main(void)
{
    int x = 100;

    pthread_t tid;

    pthread_create(&tid,   /*  consit thread id */
                   NULL,   /* config of thread */
                   worker, /* function pointer */
                   &x);    /* argument pass into function pointer */

    // pthread_join(tid, NULL);
    sleep(3);
    printf("%d\n", x);
    

    pthread_join(tid, NULL);
    return 0;
}