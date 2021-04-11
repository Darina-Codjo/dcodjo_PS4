//cd Documents/Cours/'Cours de C'/'Projet en C'/Projet/'TRY 07'
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <semaphore.h>

//Nombre total de thread
#define NB_THREAD 3

void meltButter();
void eggYolk();
void preparation();
void whip_up_egg_whites();
void waffle_iron();
void plate();
void cook();
void dressing();
void readyToEat();

//Création des semaphores
sem_t sem_meltButter;
sem_t sem_eggYolk;
sem_t sem_preparation;
sem_t sem_whip_up_egg_whites;
sem_t sem_waffle_iron;
sem_t sem_plate;
sem_t sem_cook;
sem_t sem_dressing;
sem_t sem_readyToEat;

//Recupuration de l'id du thread
int tid;

int count = 0;

//Variables globales : process, pipe, signal, buffer
pid_t butter_process;
int pipe_butter[2];
int meltedButter_sign;
int buffer_butter;

pid_t eggYolk_process; 
int pipe_eggYolk[2];
int eggYolk_sign;
int buffer_eggYolk;

pid_t eggwhites_process;
int pipe_eggWhites[2];
int whites_sign;
int buffer_eggWhites;

//Variables globales : pipe, signal, buffer
int pipe_waffleIron[2];
int waffleIron_sign;
int buffer_waffleIron;

int pipe_plate[2];
int plate_sign;
int buffer_plate;
    

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
            
            read(pipe_plate[0], &buffer_plate, sizeof(buffer_plate));// == SIGUSR1
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

void meltButter(){
    fprintf(stderr,"\npetit bol");
    fprintf(stderr," --> le beurre fond");
    sleep(2);
    fprintf(stderr," --> le beurre est pret");
    fprintf(stderr," --> envoie du beurre au grand bol...");
    meltedButter_sign = SIGUSR1;
    //close(pipe_butter[0]);//fermer lecture fils
    write(pipe_butter[1], &meltedButter_sign, sizeof(meltedButter_sign));   
    close(pipe_butter[1]);//fermer ecriture fils
    fprintf(stderr,"\n\n");
}

void eggYolk(){
    fprintf(stderr,"\nmoyen bol");
    fprintf(stderr," --> on casse les oeufs\n");
    sleep(1);
    fprintf(stderr," --> le moyen bol garde les blancs");
    fprintf(stderr," --> envoie des jaunes d'oeuf au grand bol...\n");
    eggYolk_sign = SIGUSR2;
    //close(pipe_eggYolk[0]);//fermer lecture fils
    write(pipe_eggYolk[1],&eggYolk_sign, sizeof(eggYolk_sign));
    close(pipe_eggYolk[1]);//fermer ecriture fils
    fprintf(stderr,"\n");
}

void whip_up_egg_whites(){
    fprintf(stderr,"\nmoyen bol");
    fprintf(stderr," ----> le moyen bol est pret");
    fprintf(stderr," --> monter les blancs en neige");
    sleep(2);
    fprintf(stderr," --> les blancs en neige sont prets");
    fprintf(stderr," --> envoie des blancs en neige au grand bol...\n");
    
    whites_sign = SIGUSR1;
    write(pipe_eggWhites[1],&whites_sign, sizeof(whites_sign));
    close(pipe_eggWhites[1]);//fermer ecriture fils
    fprintf(stderr,"\n\n");
}


void preparation(){
    fprintf(stderr,"\ngrand bol");
    fprintf(stderr," --> j'ai recu le beurre fondu");
    fprintf(stderr," --> j'ai recu les jaunes d'oeufs");
    fprintf(stderr," --> je melange le tout");
    sleep(1);
    fprintf(stderr," --> on ajoute le lait\n");
    fprintf(stderr," ----> on reveil le moyen bol\n");

    pipe(pipe_eggWhites);
    eggwhites_process = fork();
    if(eggwhites_process == 0){
        tid = pthread_self();
        fprintf(stderr,"\n\n***PROCESS EGG_WHITES THREAD [%i]***",tid);
        //on attend la disponibilité du sémaphore : sem_whip_up_egg_whites
        sem_wait(&sem_whip_up_egg_whites);
        //section critique
        whip_up_egg_whites();
        exit(0);
    }
    wait(NULL);
    kill(eggwhites_process,SIGKILL);

    read(pipe_eggWhites[0],&buffer_eggWhites, sizeof(buffer_eggWhites));// == SIGUSR1
    close(pipe_eggWhites[0]);//fermer lecture du pere
    

    if(buffer_eggWhites == SIGUSR1){
        //on relache le semaphore : sem_whip_up_egg_whites
        sem_post(&sem_whip_up_egg_whites);
        //on relache le semaphore : sem_eggYolk (aka le moyen bol)
        sem_post(&sem_eggYolk);
        
        tid = pthread_self();
        fprintf(stderr,"****PREPARATION PART 2 THREAD [%i]****",tid);
        fprintf(stderr,"\n--> j'ai recu les blancs en neige");
        fprintf(stderr," --> on incorpore des blancs en neige dans la préparation");
        fprintf(stderr," --> je melange le tout...\n");
        sleep(1);
        fprintf(stderr,"\n ~~~~ La préparation est prete ~~~~");
        fprintf(stderr," --> envoie de la preparation au gaufrier...\n");
        
        waffleIron_sign = SIGUSR2;
        write(pipe_waffleIron[1], &waffleIron_sign, sizeof(waffleIron_sign));// == SIGUSR2
        close(pipe_waffleIron[1]);//fermer ecriture fils
        fprintf(stderr,"\n\n");
    }
    else{
        fprintf(stderr,"\n\n WRITE ERROR : [%i] =/= {%i}\n\n", buffer_eggWhites, SIGUSR1);
        pthread_exit(0); 
    }
} 


void waffle_iron(){
    fprintf(stderr,"\ngaufrier");
    fprintf(stderr," ----> le gaufrier est pret");
    sleep(1);
    fprintf(stderr," --> le gaufrier est beurré");
    printf("\n\n");
}

void cook(){
    fprintf(stderr,"\ngaufrier");
    fprintf(stderr," --> j'ai recu la preparation");
    fprintf(stderr," --> cuisson du premier cote...");
    sleep(2);
    fprintf(stderr," --> cuisson du deuxieme cote...");
    sleep(2);
    fprintf(stderr," --> la gaufre est cuite !");
    fprintf(stderr," --> evoie de la gaufre a l'assiette...");

    plate_sign = SIGUSR1;
    write(pipe_plate[1], &plate_sign, sizeof(plate_sign));
    close(pipe_plate[1]);//fermer ecriture fils
    fprintf(stderr,"\n\n");
} 


void plate(){
    fprintf(stderr,"\nassiette");
    fprintf(stderr," ----> l'assiette se prépare");
    sleep(1);
    fprintf(stderr," --> l'assiette est prete");
    fprintf(stderr,"\n\n");
}

void dressing(){
    fprintf(stderr,"\nassiette");
    fprintf(stderr," --> j'ai recu la gaufre");
    fprintf(stderr," --> ajout du sucre glace");
    sleep(1);
    fprintf(stderr," --> la gaufre est prete a etre servie !");
    fprintf(stderr,"\n\n");
    readyToEat();
}

void readyToEat(){
    tid = pthread_self();
    counting++;
    fprintf(stderr,"!!!!!!!!!! Votre gaufre n°%i sur %i est servie !!!!!!!!!!", count,NB_THREAD);
    fprintf(stderr,"\n\n");
    tid = pthread_self();
    fprintf(stderr,"END THREAD [%i]\n\n",tid);
}

int main(){
    //creation d'un tableau de threads : nombre de gauffre a fabriquer
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
        fprintf(stderr,"\n\n ********************** VOS %i GAUFRE ONT ETE SERVIES ! ***************************\n\n",NB_THREAD);
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
