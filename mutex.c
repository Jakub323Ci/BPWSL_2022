#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int counter;
pthread_mutex_t mtx;                                            /* tworzymy zmienna typu pthread_mutex_t */

void *increment(void *arg);
void *decrement(void *arg);

int main()
{
    pthread_t th1, th2;
    counter = 0;

    pthread_mutex_init(&mtx, NULL);                             /* inicjalizacja mutexa */
    pthread_create(&th1, NULL, increment, NULL);
    pthread_create(&th2, NULL, decrement, NULL);

    pthread_join(th1, NULL);
	printf("thread 0: increment - finished\n");
	pthread_join(th2, NULL);
	printf("thread 1: decrement - finished\n");

	pthread_mutex_destroy(&mtx);                                /* zniszczenie obiektu mutex, staje sie on niezainicjalizowany */

    printf("program finished, counter is: %d\n", counter);
    return 0;
}


void *increment(void *arg)
{
    for (int i = 0; i < 10000; i++)
    {
        pthread_mutex_lock(&mtx);                               /* blokada mutexa, zwiekszenie zmiennej counter znajduje sie w sekscji krytyczenj */
        counter++;                                              /* SEKCJA KRYTYCZNA */
        pthread_mutex_unlock(&mtx);                             /* odblokowanie mutexa */
    }
    return NULL;
}

void *decrement(void *arg)
{
    for (int i = 0; i < 10000; i++)
    {
        pthread_mutex_lock(&mtx);                               /* sytuacja w przypadku funkcji decrement jest analogiczna do increment */
        counter--;
        pthread_mutex_unlock(&mtx);
    }
    return NULL;
}