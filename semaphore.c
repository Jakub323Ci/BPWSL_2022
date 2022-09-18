#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>

#define READERS 14
#define KEYS 5

sem_t receptionist;

void* reader(void* args);

int main() {
    pthread_t th[READERS];
    int i;

    srand(time(NULL));
    
    /* inicjalizacja semafora z podaniem jego wartosci poczatkowej (KEYS) */
    sem_init(&receptionist, 0, KEYS);

    for (i = 0; i < READERS; i++) {
        /* zmienna do przekazania nr-u watku jako argument funkcji obslugujacej go */
        int* a = malloc(sizeof(int));
        *a = i;

        pthread_create(&th[i], NULL, &reader, a);
    }

    for (i = 0; i < READERS; i++) {
        pthread_join(th[i], NULL);
    }

    sem_destroy(&receptionist);
    return 0;
}

void* reader(void* args) {
    printf("Reader nr %d enters library and waits for the key to a room\n", *(int*)args);

    /*
     * jezeli wartosc semafora > 0, watek przejdzie dalej, a wartosc semafora obnizy sie o 1 lub
     * jezeli wartosc semafora == 0, watek musi czekac az wartosc semafora sie zwiekszy
     */
    sem_wait(&receptionist);
    printf("Reader nr %d got a key and entered a room\n", *(int*)args);

    /* 
     * symulowane operacje przez pseudolosowa ilosc czasu
     * (czytanie ksiazki przez czytelnika w pokoju)
     */
    sleep(rand() % 7 + 1);
    printf("Reader nr %d finished reading and leaves the room\n", *(int*)args);

    /* wyjscie z sekcji chronionej semaforem i zwiekszenie jego wartosci o 1
     * (oddanie klucza do pokoju w recepcji)
     */
    sem_post(&receptionist);

    free(args);
}