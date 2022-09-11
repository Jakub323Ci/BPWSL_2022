#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>

#define START_AMOUNT	0
#define DELIVERY	100
#define CAR_CAPACITY	60

void* delivery_truck(void*);
void* car(void*);

pthread_mutex_t mtx;
pthread_cond_t cond;

int gas_pump = START_AMOUNT;
char* vehicle[4];

int main()
{
	pthread_t th[5];
	pthread_mutex_init(&mtx, NULL);
	pthread_cond_init(&cond, NULL);

	vehicle[0] = "Ford";
	vehicle[1] = "Chevrolet";
	vehicle[2] = "Dodge";
	vehicle[3] = "Pontiac";

	pthread_create(&th[0], NULL, delivery_truck, NULL);
	for (int i = 1; i < 5; i++)
	{
		pthread_create(&th[i], NULL, car, (void*)vehicle[i - 1]);
	}

	pthread_join(th[0], NULL);
	printf("\nthread 0: delivery_truck -	finished\n");
	pthread_join(th[1], NULL);
	printf("thread 1: ford -		finished\n");
	pthread_join(th[2], NULL);
	printf("thread 2: chevrolet -		finished\n");
	pthread_join(th[3], NULL); 
	printf("thread 3: pontiac -		finished\n");
	pthread_join(th[4], NULL);
	printf("thread 4: dodge -		finished\n");

	pthread_mutex_destroy(&mtx);
	pthread_cond_destroy(&cond);
}

void* delivery_truck(void *arg)
{
	for (int i = 0; i < 5; i++)
	{
		sleep(5);
		pthread_mutex_lock(&mtx);
		gas_pump += DELIVERY;
		printf("%s: fuel is now added to the gas pump (Currently %d L). Delivery truck is leaving for resupply...\n\n", __FUNCTION__, gas_pump);
		pthread_mutex_unlock(&mtx);
		pthread_cond_broadcast(&cond);		/* funkcja pthread_cond_broadcast() zawiadamia wszystkie watki oczekujace na zmienna cond */
	}

	return NULL;
}

void* car(void *arg)
{
	char *car = arg;
	printf("%s: in a queue to get refuelled\n", car);
	pthread_mutex_lock(&mtx);
	while(gas_pump < CAR_CAPACITY)
	{
		printf("%s: no fuel - still waiting...\n", car);
		pthread_cond_wait(&cond, &mtx);
	}
	gas_pump -= CAR_CAPACITY;
	printf("%s: filled up and leaves the gas station now\n", car);
	pthread_mutex_unlock(&mtx);

	return NULL;
}
