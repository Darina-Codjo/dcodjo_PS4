void meltButter();
void eggYolk();
void preparation();
void whip_up_egg_whites();
void waffle_iron();
void plate();
void cook();
void dessing();
void readyToEat();



//-----> simple exe 1 file

//gcc First.c -o first
//./first args...



//-----> medium exe 3 files : main.c, functions.c, function.h 

//gcc -c functions.c
//gcc main.c -o main function.o
//./main args...



//-----> hard exe 3 files + theads : main.c, functions.c, function.h 

//gcc -c fonctions.c
//gcc Main_Thread.c -lpthread -o main fonctions.o
//./main args...


//cd Documents/Cours/'Cours de C'/'Projet en C'/Projet