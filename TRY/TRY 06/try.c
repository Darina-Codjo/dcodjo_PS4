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

/*void signalHandler(int signalNum) {
if (signalNum == SIGUSR1) {
printf("Caught SIGUSR1 signal.\n");
}
}
int main(void) {
signal(SIGUSR1, signalHandler);
raise(SIGUSR1);
fprintf(stderr,"\n{%i}\n", SIGUSR1);
fprintf(stderr,"\n{%i}\n", SIGUSR2);
return 0;
}*/



/*int main( int argc, char ** argv ){
int buffer;

/* create the pipe 
int fd[2];
pipe(fd);
int yolk = 10 ;
int yolkFils = 0;

pid_t coucou = fork();
if(coucou == 0){
yolk = 20;
write(fd[1], &yolk,sizeof(yolk));
exit(0);
}
wait(NULL);
wait(NULL);
read(fd[0], &yolkFils, sizeof(yolkFils));
printf("\n%i\n", yolkFils);
write into the pipe 
//write(fd[1], &yolk, sizeof(yolk));

read the pipe and print the read value
// read(fd[0], &buffer, sizeof(buffer));
// fprintf(stderr,"buffer [%i]", buffer);

fprintf(stderr,"\ndoneSIG1 : {%i}\n", buffer_butter);
fprintf(stderr,"\ndoneSIG2 : {%i}\n", buffer_egg);
fprintf(stderr,"\n{%i}\n", SIGUSR1);
fprintf(stderr,"\n{%i}\n", SIGUSR2);
}*/
