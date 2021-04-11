//   cd Documents/Cours/'Cours de C'/'Projet en C'/Projet/'TRY 07'
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <memory.h>
#include <unistd.h>

int tid = pthread_self();

struct threadList {
  pthread_t *thread;
  int *rank;
};

struct str list;

struct threadList thread1 = {} 

*********************************************




typedef struct element element;
struct element{
    int val;
    struct element *nxt;
};
typedef element* threadList;

 

threadList ajouterEnFin(threadList liste, int valeur){
    /* On crée un nouvel élément */
    element* nouvelElement = malloc(sizeof(element));
 
    /* On assigne la valeur au nouvel élément */
    nouvelElement->val = valeur;
 
    /* On ajoute en fin, donc aucun élément ne va suivre */
    nouvelElement->nxt = NULL;
 
    if(liste == NULL){
        /* Si la liste est videé il suffit de renvoyer l'élément créé */
        return nouvelElement;
    }
    else{
        /* Sinon, on parcourt la liste à l'aide d'un pointeur temporaire et on
        indique que le dernier élément de la liste est relié au nouvel élément */
        element* temp=liste;
        while(temp->nxt != NULL)        {
            temp = temp->nxt;
        }
        temp->nxt = nouvelElement;
        return liste;
    }
}

threadList supprimerElementEnTete(threadList liste){
    if(liste != NULL)    {
        /* Si la liste est non vide, on se prépare à renvoyer l'adresse de
        l'élément en 2ème position */
        element* aRenvoyer = liste->nxt;
        /* On libère le premier élément */
        free(liste);
        /* On retourne le nouveau début de la liste */
        return aRenvoyer;
    }
    else{
        return NULL;
    }
}


threadList rechercherElement(threadList liste, int valeur){
    element *tmp=liste;
    /* Tant que l'on n'est pas au bout de la liste */
    while(tmp != NULL)    {
        if(tmp->val == valeur)        {
            /* Si l'élément a la valeur recherchée, on renvoie son adresse */
            return tmp;
        }
        tmp = tmp->nxt;
    }
    return NULL;
}


int nombreElements(threadList liste){
    /* Si la liste est vide, il y a 0 élément */
    if(liste == NULL)
        return 0;
 
    /* Sinon, il y a un élément (celui que l'on est en train de traiter)
    plus le nombre d'éléments contenus dans le reste de la liste */
    return nombreElements(liste->nxt)+1;
}


int estVide(threadListe liste){
    if(liste == NULL){
        return 1;
    }
    else{
        return 0;
    }
}


threadList effacerListe(threadList liste){
    if(liste == NULL){
        /* Si la liste est vide, il n'y a rien à effacer, on retourne 
        une liste vide i.e. NULL */
        return NULL;
    }
    else{
        /* Sinon, on efface le premier élément et on retourne le reste de la 
        liste effacée */
        element *tmp;
        tmp = liste->nxt;
        free(liste);
        return effacerListe(tmp);
    }
}


int main(int argc, char **argv){
    /* Déclarons 3 listes chaînées de façons différentes mais équivalentes */
    threadList ma_liste1 = NULL;
    element *ma_liste2 = NULL;
    struct element *ma_liste3 = NULL;
    return 0;
}

int main(int argc, char **argv){
    threadList ma_liste = NULL;
    int i;
 
    for(i=1;i<=10;i++)    {
        ma_liste = ajouterEnTete(ma_liste, i);
        ma_liste = ajouterEnFin(ma_liste, i);
    }
    afficherListe(ma_liste);
 
    supprimerListe(ma_liste); // Libère les ressources, nous verrons cette fonction plus tard.
 
    return 0;
}

/*if(estVide(ma_liste))
    {
        printf("La liste est vide");
    }
    else
    {
        afficherListe(ma_liste);
    }
*/











