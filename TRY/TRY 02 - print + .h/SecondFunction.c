#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "Second.h"


void meltButter(){
    printf("petit bol");
    printf(" --> le beurre fond");
    sleep(2);
    printf(" --> le beurre est pret");
    printf(" --> envoie du beurre au grand bol...");
    printf("\n\n");
}

void eggYolk(){
    printf("moyen bol");
    printf(" --> on casse les oeufs");
    sleep(1);
    printf(" --> le moyen bol gardes les blancs");
    printf(" --> envoie des jaunes d'oeuf au grand bol...");
    printf("\n\n");
}

void preparation(){
    printf("grand bol");
    printf(" --> j'ai recu le beurre fondu");
    printf(" --> j'ai recu les jaunes d'oeufs");
    printf(" --> je melange le tout");
    sleep(1);
    printf(" --> on ajoute le lait\n");

    printf("  ----> on reveil le moyen bol\n");
        whip_up_egg_whites();

    printf("--> j'ai recu les blancs en neige\n");

    printf("\n  ----> on reveil le gaufrier\n");
        waffle_iron();

    printf("  ----> on reveil l'assiette\n");
        plate();

    printf(" --> on incorpore des blancs en neige dans la préparation");
    printf(" --> je melange le tout");
    sleep(1);
    printf("\n\nLa préparation est prete !\n");
    printf(" --> envoie de la preparation au gaufrier");
    printf("\n\n");

}

void whip_up_egg_whites(){
    printf("    moyen bol");
    printf(" ----> le moyen bol est pret");
    printf(" --> monter les blancs en neige");
    sleep(2);
    printf(" --> les blancs en neige sont prets");
    printf(" --> envoie des blancs d'oeuf au grand bol...");
    printf("\n\n");
}

void waffle_iron(){
    printf("    gaufrier");
    printf(" ----> le gaufrier est pret");
    sleep(1);
    printf(" --> le gaufrier est beurré");
    printf("\n\n");
}

void plate(){
    printf("    assiette");
    printf(" ----> l'assiette se prépare");
    sleep(1);
    printf(" --> l'assiette est prete");
    printf("\n\n");
}

void cook(){
    printf("    gaufrier");
    printf(" --> j'ai recu la preparation");
    printf(" --> cuisson du premier cote...");
    sleep(2);
    printf(" --> cuisson du deuxieme cote...");
    sleep(2);
    printf(" --> la gaufre est cuite !");
    printf(" --> evoie de la gaufre a l'assiette...");
    printf("\n\n");
}

void dessing(){
    printf("    assiette");
    printf(" --> j'ai recu la gaufre");
    printf(" --> ajout du sucre glace");
    sleep(1);
    printf(" --> la gaufre est prete a etre servie");
    printf("\n\n");
    readyToEat();
}

void readyToEat(){
    printf("Votre gaufre esr servie !");
    printf("\n\n");
    printf("END\n\n");
}