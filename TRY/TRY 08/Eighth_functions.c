//cd Documents/Cours/'Cours de C'/'Projet en C'/Projet/'TRY 08'
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <semaphore.h>
#include "Eighth.h"


//**********************************************************************************************************************************************************************************************************************************************************************************************

void meltButter(){
    fprintf(stderr,"\npetit bol");
    fprintf(stderr," --> le beurre fond");
    sleep(3);
    fprintf(stderr," --> le beurre est pret");
    fprintf(stderr," --> envoie du beurre au grand bol...");
    meltedButter_sign = SIGUSR1;
    //close(pipe_butter[0]);//fermer lecture fils
    write(pipe_butter[1], &meltedButter_sign, sizeof(meltedButter_sign));   
    close(pipe_butter[1]);//fermer ecriture fils
    fprintf(stderr,"\n\n");
}

//**********************************************************************************************************************************************************************************************************************************************************************************************

void eggYolk(){
    fprintf(stderr,"\nmoyen bol");
    fprintf(stderr," --> on casse les oeufs\n");
    sleep(2);
    fprintf(stderr," --> le moyen bol garde les blancs");
    fprintf(stderr," --> envoie des jaunes d'oeuf au grand bol...\n");
    eggYolk_sign = SIGUSR2;
    //close(pipe_eggYolk[0]);//fermer lecture fils
    write(pipe_eggYolk[1],&eggYolk_sign, sizeof(eggYolk_sign));
    close(pipe_eggYolk[1]);//fermer ecriture fils
    fprintf(stderr,"\n");
}

//**********************************************************************************************************************************************************************************************************************************************************************************************

void whip_up_egg_whites(){
    fprintf(stderr,"\nmoyen bol");
    fprintf(stderr," ----> le moyen bol est pret");
    fprintf(stderr," --> monter les blancs en neige");
    sleep(3);
    fprintf(stderr," --> les blancs en neige sont prets");
    fprintf(stderr," --> envoie des blancs en neige au grand bol...\n");
    
    whites_sign = SIGUSR1;
    write(pipe_eggWhites[1],&whites_sign, sizeof(whites_sign));
    close(pipe_eggWhites[1]);//fermer ecriture fils
    fprintf(stderr,"\n\n");
}

//**********************************************************************************************************************************************************************************************************************************************************************************************

void preparation(){
    fprintf(stderr,"\ngrand bol");
    fprintf(stderr," --> j'ai recu le beurre fondu");
    fprintf(stderr," --> j'ai recu les jaunes d'oeufs");
    fprintf(stderr," --> je melange le tout");
    sleep(2);
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
        sleep(2);
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

//**********************************************************************************************************************************************************************************************************************************************************************************************

void waffle_iron(){
    fprintf(stderr,"\ngaufrier");
    fprintf(stderr," ----> le gaufrier est pret");
    sleep(2);
    fprintf(stderr," --> le gaufrier est beurré");
    printf("\n\n");
}

//**********************************************************************************************************************************************************************************************************************************************************************************************

void cook(){
    fprintf(stderr,"\ngaufrier");
    fprintf(stderr," --> j'ai recu la preparation");
    fprintf(stderr," --> cuisson du premier cote...");
    sleep(3);
    fprintf(stderr," --> cuisson du deuxieme cote...");
    sleep(3);
    fprintf(stderr," --> la gaufre est cuite !");
    fprintf(stderr," --> evoie de la gaufre a l'assiette...");

    plate_sign = SIGUSR1;
    write(pipe_plate[1], &plate_sign, sizeof(plate_sign));
    close(pipe_plate[1]);//fermer ecriture fils
    fprintf(stderr,"\n\n");
} 

//**********************************************************************************************************************************************************************************************************************************************************************************************

void plate(){
    fprintf(stderr,"\nassiette");
    fprintf(stderr," ----> l'assiette se prépare");
    sleep(2);
    fprintf(stderr," --> l'assiette est prete");
    fprintf(stderr,"\n\n");
}

//**********************************************************************************************************************************************************************************************************************************************************************************************

void dressing(){
    fprintf(stderr,"\nassiette");
    fprintf(stderr," --> j'ai recu la gaufre");
    fprintf(stderr," --> ajout du sucre glace");
    sleep(2);
    fprintf(stderr," --> la gaufre est prete a etre servie !");
    fprintf(stderr,"\n\n");
    readyToEat();
}

//**********************************************************************************************************************************************************************************************************************************************************************************************

void readyToEat(){
    tid = pthread_self();
    counting++;
    fprintf(stderr,"!!!!!!!!!! Votre gaufre n°%i sur %i est servie !!!!!!!!!!", counting,NB_THREAD);
    fprintf(stderr,"\n\n");
    tid = pthread_self();
    fprintf(stderr,"END THREAD [%i]\n\n",tid);
}