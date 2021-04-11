#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <semaphore.h>
#include "Eighth.h"

void * threadJob(void * args){
    //Recupuration de l'id du thread
    tid = pthread_self();
    fprintf(stderr,"\n Thread --> [%i]\n",tid);

    //on attend la disponibilité du sémaphore : eggYolk
    sem_wait(&sem_eggYolk);
    //section critique
    pipe(pipe_eggYolk);
    eggYolk_process = fork();
    if(eggYolk_process == 0){
        tid = pthread_self();
        fprintf(stderr,"\n\n****PROCESS EGG_YOLK THREAD [%i]****",tid);
        eggYolk();
        exit(0);       
    }

    //on attend la disponibilité du sémaphore : sem_meltButter
    sem_wait(&sem_meltButter);
    //section critique
    pipe(pipe_butter);
    butter_process = fork();
    if(butter_process == 0){
        tid = pthread_self();
        fprintf(stderr,"\n\n****PROCESS BUTTER THREAD [%i]****",tid);
        meltButter();
        exit(0);
    }

    wait(NULL);
    wait(NULL);
    
    kill(butter_process,SIGKILL);
    kill(eggYolk_process,SIGKILL);    
    
    read(pipe_butter[0],&buffer_butter, sizeof(buffer_butter));// == SIGUSR1
    read(pipe_eggYolk[0],&buffer_eggYolk, sizeof(buffer_eggYolk));// == SIGUSR2
    
    close(pipe_butter[0]);//fermer lecture du pere
    close(pipe_eggYolk[0]);//fermer lecture du pere
    
    if(buffer_butter == SIGUSR1 && buffer_eggYolk == SIGUSR2){
        //on relache le semaphore : sem_meltButter
        sem_post(&sem_meltButter);
        //on ne relache PAS ENCORE le semaphore : sem_eggYolk

        tid = pthread_self();
        fprintf(stderr,"\n****PREPARATION THREAD [%i]****",tid);
        //on attend la disponibilité du sémaphore : sem_preparation
        sem_wait(&sem_preparation);
        //section critique
        pipe(pipe_waffleIron);

        preparation();
        //--> write in pipe_waffle

        //on relache le semaphore : sem_preparation
        sem_post(&sem_preparation);

        tid = pthread_self();
        fprintf(stderr,"****WAFFLE_IRON THREAD [%i]****",tid);
        //on attend la disponibilité du sémaphore : sem_waffle_iron
        sem_wait(&sem_waffle_iron);
        //section critique
        waffle_iron();
        
        read(pipe_waffleIron[0], &buffer_waffleIron, sizeof(buffer_waffleIron));// == SIGUSR2
        close(pipe_waffleIron[0]);//fermer lecture du pere  

              
        if(buffer_waffleIron == SIGUSR2){
            tid = pthread_self();
            fprintf(stderr,"\n****COOK THREAD [%i]****",tid);  

            //on attend la disponibilité du sémaphore : sem_cook 
            sem_wait(&sem_cook);
            //section critique
            pipe(pipe_plate);
            cook();
            //--> write in pipe_plate
            
            //read(pipe_plate[0], &buffer_plate, sizeof(buffer_plate));// == SIGUSR1
            close(pipe_plate[0]);//fermer lecture du pere

            if(buffer_plate == SIGUSR1){
                tid = pthread_self();
                fprintf(stderr,"\n****PROCESS PLATE THREAD [%i]****",tid);

                //on attend la disponibilité du sémaphore : sem_plate
                sem_wait(&sem_plate);
                //section critique

                //on relache le semaphore : sem_cook
                sem_post(&sem_cook);
                //on relache le semaphore : sem_waffle_iron
                sem_post(&sem_waffle_iron);
                
                plate();

                tid = pthread_self();
                fprintf(stderr,"\n****PROCESS DRESSING THREAD [%i]****",tid);
                //on attend la disponibilité du sémaphore : sem_dessing 
                sem_wait(&sem_dressing);
                //section critique
                dressing();

                //on relache le semaphore : sem_dressing
                sem_post(&sem_dressing);
                //on relache le semaphore : sem_plate
                sem_post(&sem_plate);
            }
            else{
                tid = pthread_self();
                fprintf(stderr,"\n\n====================>> WRITE ERROR : {%i} =/= {%i}, buffer_plate =/= SIGUSR1 IN --> [%i] <<====================\n", buffer_plate, SIGUSR1,tid);
                fprintf(stderr,"====================>> DESTRUCTION DU PROCESSUS...\n\n");
                kill(getpid(), SIGPIPE); //== erreur d'ecriture dans le pipe --> mise à mort du processus
                
                /*SIGINT //== ^C , mise à mort propre du programme
                SIGPIPE //== erreur d'ecriture dans le pipe --> mise à mort du processus*/
            }
        }
        else{
            tid = pthread_self();
            fprintf(stderr,"\n\n====================>> WRITE ERROR : {%i} =/= {%i}, buffer_waffleIron =/= SIGUSR2 IN --> [%i]<<====================\n\n", buffer_waffleIron, SIGUSR2,tid);
            fprintf(stderr,"====================>> DESTRUCTION DU PROCESSUS...\n\n");
            kill(getpid(), SIGPIPE); //== erreur d'ecriture dans le pipe --> mise à mort du processus
        }
    }
    else{
        tid = pthread_self();
        fprintf(stderr,"\n\n====================>> WRITE ERROR : {%i} =/= {%i}, buffer_butter =/= SIGUSR1 IN --> [%i] <<=========\n =========>> WRITE ERROR : {%i} =/= {%i}, buffer_eggYolk =/= SIGUSR2 IN --> [%i] <<=========\n\n",buffer_butter,SIGUSR1,tid, buffer_eggYolk,SIGUSR2,tid);
        fprintf(stderr,"====================>> DESTRUCTION DU PROCESSUS...\n\n");
        kill(getpid(), SIGPIPE); //== erreur d'ecriture dans le pipe --> mise à mort du processus
    }
    pthread_exit(EXIT_SUCCESS);
}


int main(){
    //creation d'un tableau de threads : nombre de gauffre a fabriquer
    printf("Saisissez le nombre de gaufre que vous voulez faire entre 1 et 10... \nNOMBRE DE GAUFRE(S) : ");
    scanf("%i",&NB_THREAD);
    sleep(1);
    pthread_t threads[NB_THREAD]; 
    int error_creation;
    int error_join;
    int ii = 0;

    //init des semaphores
    sem_init(&sem_meltButter, PTHREAD_PROCESS_SHARED,1);
    sem_init(&sem_eggYolk, PTHREAD_PROCESS_SHARED,1);
    sem_init(&sem_preparation, PTHREAD_PROCESS_SHARED,1);
    sem_init(&sem_whip_up_egg_whites, PTHREAD_PROCESS_SHARED,1);
    sem_init(&sem_waffle_iron, PTHREAD_PROCESS_SHARED,1);
    sem_init(&sem_cook, PTHREAD_PROCESS_SHARED,1);
    sem_init(&sem_plate, PTHREAD_PROCESS_SHARED,1);
    sem_init(&sem_dressing, PTHREAD_PROCESS_SHARED,1);
    sem_init(&sem_readyToEat, PTHREAD_PROCESS_SHARED,1);

    for(int i = 0; i < NB_THREAD; i++){
        error_creation = pthread_create(&threads[i], NULL, threadJob, NULL);
        if(error_creation != 0){
            fprintf(stderr,"ECHEC PTHREAD_CREATE");
            return EXIT_FAILURE;
        }
    }

    for(int i = 0; i < NB_THREAD; i++){
        error_join = pthread_join(threads[i], NULL);
        if(error_join != 0){
            fprintf(stderr,"ECHEC PTHREAD_JOIN"); 
            return EXIT_FAILURE;
        }
        ii++;
    }

    if(ii == NB_THREAD){
        if(NB_THREAD > 1){
            fprintf(stderr,"\n ********************** VOS %i GAUFRES ONT ETE SERVIES ! ***************************\n\n",NB_THREAD);
        }
        else{
            fprintf(stderr,"\n ********************** VOTRE GAUFRE A ETE SERVIE ! ***************************\n\n");
        }
    }
    
    //destruction des sémaphore
    sem_destroy(&sem_meltButter);
    sem_destroy(&sem_eggYolk);
    sem_destroy(&sem_preparation);
    sem_destroy(&sem_whip_up_egg_whites);
    sem_destroy(&sem_waffle_iron);
    sem_destroy(&sem_cook);
    sem_destroy(&sem_plate);
    sem_destroy(&sem_dressing);
    sem_destroy(&sem_readyToEat);

    return EXIT_SUCCESS;
}