//cd Documents/Cours/Cours de C/Projet en C/Projet/TRY 04
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <semaphore.h>

//Nombre total de thread
#define NB_THREAD 2

//Création du semaphore
sem_t semaphore;

void * threadJob(void * args){
    //Recupuration de l'id du thread
    int tid = pthread_self();
    int i = 0;
    //on attend la disponibilité du sémaphore
    sem_wait(&semaphore);
    //section critique
    printf("Je suis le thread [%i] et je dort 3 seconde\n", tid);
    sleep(3);
    printf("Je suis le thread [%i] et je suis reveillé\n", tid);
    //on relache le semaphore
    sem_post(&semaphore);
    i++;
    pthread_exit(EXIT_SUCCESS);
}

int main(){
    //creation d'un tableau de threads
    pthread_t threads[NB_THREAD];
    int error;
    //init du semaphore
    sem_init(&semaphore, PTHREAD_PROCESS_SHARED,1);
    for(int i = 0; i < NB_THREAD; i++){
        
        printf("Creation du thread numero : [%i]\n", i);
        error = pthread_create(&threads[i], NULL, threadJob, NULL);

        if(error != 0){
            printf("Echec de la creation du thread");
            return EXIT_FAILURE;
        }
    }

    for(int i = 0; i < NB_THREAD; i++){
        pthread_join(threads[i], NULL);
    }
    sem_destroy(&semaphore);
    return EXIT_SUCCESS;
}