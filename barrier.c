#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define HORSES_QTY 15

/* zdefiniowanie zmiennych typu pthread_barrier_t */
pthread_barrier_t starting_gate;
pthread_barrier_t start_signal;

void* horse(void* args);

int main() {
	pthread_t th[HORSES_QTY];
	int i;

    srand(time(NULL));
    /* inicjalizacja barier z wartosciami rownymi ilosci watkow potomnych + watek glowny */
    pthread_barrier_init(&starting_gate, NULL, HORSES_QTY+1);
    pthread_barrier_init(&start_signal, NULL, HORSES_QTY+1);

	for (i = 0; i < HORSES_QTY; i++) {
        int* a = malloc(sizeof(int));
        *a = i;
		pthread_create(&th[i], NULL, &horse, a);
	}

    /*
     * wyswietlenie komunikatu o koniach przygotowujacych sie do wyscigu;
     * synchronizacja watkow (koni) przed wyscigiem;
     * rozpoczecie wyscigu (watek glowny wyswietla komunikat);
     * synchronizacja watkow poczatku wyscigu (oczekiwanie koni na sygnal do startu);
     */

    printf("All horses preparing to race\n");
    pthread_barrier_wait(&starting_gate);
    printf("START!\n");
    pthread_barrier_wait(&start_signal);

    /* oczekiwanie na zakonczenie pracy watkow */
	for (i = 0; i < HORSES_QTY; i++) {
		pthread_join(th[i], NULL);
	}

    /* zniszczenie obiektu bariery */
    pthread_barrier_destroy(&starting_gate);
    pthread_barrier_destroy(&start_signal);
	return 0;
}

void* horse(void* args) {
    sleep(rand() % 10 + 2);
    printf("[%d] Horse ready, waiting at the starting gate\n", *(int*)args);
    
    /* zatrzymanie watkow (koni) w tym samym miejscu po tym kiedy sa gotowe do wyscigu */
    pthread_barrier_wait(&starting_gate);
    /* druga bariera potrzebna jest do tego by watek glowny najpierw wyswietlil sygnal do startu */
    pthread_barrier_wait(&start_signal);
    printf("[%d] Horse started running!\n", *(int*)args);
    usleep(rand() % 500000 + 2000000);
    printf("[%d] Horse crossed the finish line\n", *(int*)args);
}