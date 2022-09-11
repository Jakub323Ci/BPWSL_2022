#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>

#define INCOME 4000
#define THIEFS_TAKE 60000
#define START_BALANCE 20000

void* generate_income(void*);
void* thief(void*);

pthread_mutex_t mtx;
pthread_cond_t cond;

int money = START_BALANCE;

int main()
{
	pthread_t th1, th2;
	pthread_mutex_init(&mtx, NULL);
	pthread_cond_init(&cond, NULL);									// inicjalizacja zmiennej warunkowej

	pthread_create(&th1, NULL, thief, NULL);
	pthread_create(&th2, NULL, generate_income, NULL);

	pthread_join(th1, NULL);
	printf("thread 1: thief - finished\n");
	pthread_join(th2, NULL);
	printf("thread 2: generate_income - finished\n");

	pthread_mutex_destroy(&mtx);
	pthread_cond_destroy(&cond);									// zniszczenie zmiennej warunkowej
}

void* generate_income(void *arg)									// funkcja symulujaca dochod comiesieczny
{
	for(int i = 0; i < 20; i++)										// 20-krotne wywolanie rownowazne 20 miesiacom zarobkow
	{
		pthread_mutex_lock(&mtx);									// zablokowanie mutexa, aby bezpiecznie operowac na zmiennej globalnej w sekcji krytycznej

		money += INCOME;											// dodanie wynagrodzenia na konto
		assert(money != 0);
		printf("%s: account balance: %d\n",__FUNCTION__, money);

		pthread_mutex_unlock(&mtx);									// odblokowanie mutexa
		pthread_cond_signal(&cond);									// sygnalizujemy zmienna warunkowa, aby sprawdzic czy warunek nie zostal spelniony

		sleep(1);
	}

	return NULL;
}

void* thief(void *arg)
{
	pthread_mutex_lock(&mtx);										// blokowanie mutexa do operacji na sekcji krytycznej
	while(money < THIEFS_TAKE)										// petla while, w ktorej jest wlasciwy WARUNEK, w tym przypadku funkcja
	{																// pthread_cond_wait() jest wykonywana zawsze, gdy pieniedzy na koncie jest mniej niz
		pthread_cond_wait(&cond, &mtx);								// w makrze THIEFS_TAKE
	}

	money -= THIEFS_TAKE;											// odjecie skradzionych pieniedzy z konta bankowego
	printf("%s: thief stole the money!\n", __FUNCTION__);

	pthread_mutex_unlock(&mtx);										// odblokowanie mutexa

	return NULL;
}
