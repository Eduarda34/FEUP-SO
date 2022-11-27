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

int processWaitingRoom(char* fifo, int current, int next, int n, float p, int t){
  //printf("%s -> %d\n", fifo, getpid()); 
  

  //comeca a correr o rng do random()
  //isto é necessario para que o valor do random() seja diferente em cada processo
  //https://stackoverflow.com/questions/35641747/why-does-each-child-process-generate-the-same-random-number-when-using-rand
  time_t tt;
  srand((int)time(&tt) % getpid());

  char token[20];

  char* readingFifo = fifo;
  char writingFifo[strlen(fifo)];
  if(next == n) snprintf(writingFifo, strlen(fifo) + 1, "pipe%dto%d", next, 1);
  else 		snprintf(writingFifo, strlen(fifo) + 1, "pipe%dto%d", next, next+1);


  while(1){

    int fd;
    fd = open(readingFifo, O_RDONLY);
    //if(fd == -1) break;
    read(fd, token, 20);
    close(fd);

    
    int tokenInt = atoi(token) + 1;
    int newDecimal = 0;
    if(token[strlen(token) - 1] == '9') newDecimal = 1;
    snprintf(token, strlen(token) + 1 + newDecimal, "%d", tokenInt);
   
    float jail = ((float) (rand() % 10000))/10000;
    

    //printf("jail: %f\n", jail);
    //printf("p: %f\n", p);

    //printf("CURRENT: %d;\ttoken: %d;\tnextfifo: %s\tpid: %d\n", current, tokenInt - 1, fifo, getpid());

    if(jail < p){
      printf("[p%d] lock on token (val = %s)\n", current, token);
      //printf("jail: %f\n", jail);
      //printf("p: %f\n", p);
      sleep(t);
      printf("[p%d] unlock token\n", current);
    }

    fd = open(writingFifo, O_WRONLY);
    write(fd, token, 20);
    close(fd);
    //if(tokenInt + (n - current) > 100){
      //break;
    //}
    //if(tokenInt >= 100) break;
  }
  

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

    if(fork() == 0){
      int current = i;
      int next;
      if( i == n ) next = 1;
      else next = i +1;

      //printf("child\t[pid %d]\nmaking pipe%dto%d\n", getpid(), current, next);
      //printf("making pipe%dto%d\n", current, next);
      
      char pipeName[20];

      snprintf(pipeName, sizeof(pipeName), "pipe%dto%d", current, next);

      printf("creating %s\n", pipeName);

      int fd = mkfifo(pipeName, 0666);
      //printf("fd: %d\n", fd);

      processWaitingRoom(pipeName, current, next, n, p, t);

      exit(0);
    }else{
      //printf("parent\t[pid %d]\n", getpid());
      //printf("parent\t\tpid: %d\n", getpid());
    }

    //sleep(2);
  
  }

  sleep(2);


  char fifo[20] = "";
  snprintf(fifo, sizeof(fifo), "pipe%dto%d", 1, 2);
  //printf("%s\n", fifo);

  int fd = open(fifo,O_WRONLY);
  write(fd, "0", strlen("0"));
  close(fd);







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

