#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "Second.h"

int main(int argc, char ** argv){
    
    /*int numberWaffle;
    printf("Entrez le nopbre de gaufre : ");
    scanf("%d", &numberWaffle);
    printf("\n");
    printf("Vous avez demandez %d gaufre(s)\n", numberWaffle);*/

    printf("\nVous avez demandé 1 gaufre\n\n");

    meltButter();
    eggYolk();
    preparation();
    cook();
    dessing();
}