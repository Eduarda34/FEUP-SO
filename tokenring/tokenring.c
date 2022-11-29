#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>

int processWaitingRoom(char* readingFifo, char* writingFifo, int current, int n, float p, int t){

  //comeca a correr o rng do random()
  //isto é necessario para que o valor do random() seja diferente em cada processo
  //https://stackoverflow.com/questions/35641747/why-does-each-child-process-generate-the-same-random-number-when-using-rand
  time_t tt;
  srand((int)time(&tt) % getpid());

  char token[20];

  int readingFd;
  int writingFd;
    
  if(current == 1){
    sleep(2); 
    writingFd = open(writingFifo , O_WRONLY);
    write(writingFd, "0", strlen("0") + 1);
  }

  readingFd = open(readingFifo, O_RDONLY);
  writingFd = open(writingFifo, O_WRONLY);
    
  while(1){

    read(readingFd, token, 20);
    
    int tokenInt = atoi(token) + 1;
    int newDecimal = 0;
    if(token[strlen(token) - 1] == '9') newDecimal = 1;
    snprintf(token, strlen(token) + 1 + newDecimal, "%d", tokenInt);
   


    float jail = ((float) (rand() % 10000))/10000;

    if(jail < p){
      printf("[p%d]\tlock on token (val = %s)\n", current, token);
      sleep(t);
      printf("[p%d]\tunlock token\n", current);
    }
    
    write(writingFd, token, 20);
  }
  
  close(readingFd);
  close(writingFd);


  printf("end of child\tpid: %d\n", getpid());
    
  return 0; 
}






int main(int argc, char* argv[]){

  int n = atoi(argv[1]);
  float p = atof(argv[2]);
  int t = atoi(argv[3]);

  if(p > 1 || p < 0){
    printf("p must be between 0 and 1 ");
    return EXIT_FAILURE;
  }

  for(int i = 1; i <= n; i++){

    int current = i;
    int next;
    int before;
    
    if( i == n ) 	next = 1;
    else 		next = i + 1;
    
    if(i == 1) 		before = n;
    else 	 	before = current - 1;

    char readingFifo[20];
    char writingFifo[20];

    snprintf(readingFifo, sizeof(readingFifo), "pipe%dto%d", before, current);
    snprintf(writingFifo, sizeof(writingFifo), "pipe%dto%d", current, next);

    //printf("creating %s\n", writingFifo);

    int fd = mkfifo(writingFifo, 0666);

    if(fork() == 0){
      processWaitingRoom(readingFifo, writingFifo, current, n, p, t);

      exit(0);
    }

  }




  /* Wait for children */
  int corpse;
  int status; // if stattus == 0 then great success 
  while ((corpse = wait(&status)) > 0){
      
      if(status == 1){
	printf("FAILLLLLLEDDDDDD");
	return EXIT_FAILURE;
      }

  }

  printf("\nTHE END...\n");
  return EXIT_SUCCESS;

}

