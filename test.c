#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>
#define NTHREAD 3

pthread_cond_t buffer_cv = PTHREAD_COND_INITIALIZER;
pthread_mutex_t buffer_key = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t buffer2_key = PTHREAD_MUTEX_INITIALIZER;

void *child_thread1( void *arg )
{
    int *num = NULL;

    num = (int *)arg;
    pthread_mutex_lock(&buffer_key);
    printf("CHILD THREAD 1: num = %d\n", *num);
    *num = *num + 50;
    pthread_mutex_unlock(&buffer_key);

    /* Signal the parent thread */
    pthread_cond_signal(&buffer_cv);    
}

void *child_thread2( void *arg )
{
    int *num = NULL;

    num = (int *)arg;
    pthread_mutex_lock(&buffer_key);
    printf("CHILD THREAD 2: num = %d\n", *num);
    *num = *num + 50;
    pthread_mutex_unlock(&buffer_key);

    /* Signal the parent thread */
    pthread_cond_signal(&buffer_cv);    
}

void *parent_thread( void *arg )
{
    int *num = NULL;

    num = (int *)arg;
    *num = *num + 10;


    /* Block the parent thread */
    if ( *num == 10 )
        pthread_cond_wait(&buffer_cv, &buffer_key);

    printf("PARENT THREAD: num = %d\n", *num);

    return NULL;
}

int main()
{
    int count = 0;
    pthread_t threads[NTHREAD];

    /* Creating Parent Thread */
    pthread_create( &threads[0], NULL, parent_thread, &count );

    /* Creating Child Thread */
    pthread_create( &threads[1], NULL, child_thread1, &count );
    pthread_create( &threads[2], NULL, child_thread2, &count );
    
    pthread_join( threads[0], NULL );
    pthread_join( threads[1], NULL );
    pthread_join( threads[2], NULL );
    return 0;
}
