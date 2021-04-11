//Nombre total de thread
int NB_THREAD;

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

int counting;

//Recupuration de l'id du thread
int tid;

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

void * threadJob(void * args);
void meltButter();
void eggYolk();
void preparation();
void whip_up_egg_whites();
void waffle_iron();
void plate();
void cook();
void dressing();
void readyToEat();




//      gcc -c Eighth_functions.c
//      gcc Eighth_main.c -lpthread -o main Eighth_functions.o
//      ./main