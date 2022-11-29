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
  //printf("%s -> %d\n", fifo, getpid()); 
  

  //comeca a correr o rng do random()
  //isto é necessario para que o valor do random() seja diferente em cada processo
  //https://stackoverflow.com/questions/35641747/why-does-each-child-process-generate-the-same-random-number-when-using-rand
  time_t tt;
  srand((int)time(&tt) % getpid());

  char token[20];

  //printf("[p%d] readingFifo: %s\n", current, readingFifo);
  //printf("[p%d] writingFifo: %s\n", current, writingFifo);


  printf("waiting room\n");

  sleep(3); 

  int readingFd;
  int writingFd;
  printf("[p%d] readingFifo: %s\n", current, readingFifo);
  printf("[p%d] writingFifo: %s\n", current, writingFifo);
    
  if(current == 1){
    printf("start of cycle\n");
    sleep(3);
    writingFd = open(writingFifo , O_WRONLY);
    write(writingFd, "0", strlen("0") + 1);
  }

  readingFd = open(readingFifo, O_RDONLY);
  printf("read Fifo\n");
  writingFd = open(writingFifo , O_WRONLY);
  printf("written Fifo\n");
    

  while(1){

    //printf("[p%d] beggining of loop\n", current);

    //int fd;
    //fd = open(readingFifo, O_RDONLY);
    //if(fd == -1) break;
    read(readingFd, token, 20);
    //if(strlen(token) == 0) continue;
    //printf("[p%d] after read\n", current);
    //close(fd);

    
    int tokenInt = atoi(token) + 1;
    int newDecimal = 0;
    if(token[strlen(token) - 1] == '9') newDecimal = 1;
    snprintf(token, strlen(token) + 1 + newDecimal, "%d", tokenInt);
    //printf("[p%d] token: %s\n", current, token);
   
    float jail = ((float) (rand() % 10000))/10000;
    

    //printf("jail: %f\n", jail);
    //printf("p: %f\n", p);

    //printf("CURRENT: %d;\ttoken: %d;\tnextfifo: %s\tpid: %d\n", current, tokenInt - 1, fifo, getpid());

    if(jail < p){
      printf("[p%d]\tlock on token (val = %s)\n", current, token);
      //printf("jail: %f\n", jail);
      //printf("p: %f\n", p);
      sleep(t);
      //usleep(500);
      printf("[p%d]\tunlock token\n", current);
    }
    
    //printf("[p%d] popo\n", current);
    //fd = open(writingFifo, O_WRONLY);
    //printf("[p%d] popo2\n", current);
    write(writingFd, token, 20);
    //printf("[p%d] popo3\n", current);
    //close(fd);
    //printf("[p%d] popo4\n", current);
    //if(tokenInt + (n - current) > 100){
      //break;
    //}
    //if(tokenInt >= 100) break;
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
    else 		next = i +1;
    
    if(i == 1) 	before = n;
    else 	 	before = current - 1;

    //printf("child\t[pid %d]\nmaking pipe%dto%d\n", getpid(), current, next);
    //printf("making pipe%dto%d\n", current, next);
    
    char readingFifo[20];
    char writingFifo[20];

    snprintf(readingFifo, sizeof(readingFifo), "pipe%dto%d", before, current);
    snprintf(writingFifo, sizeof(writingFifo), "pipe%dto%d", current, next);

    printf("creating %s\n", writingFifo);

    int fd = mkfifo(writingFifo, 0666);
    //printf("fd: %d\n", fd);

    if(fork() == 0){
      processWaitingRoom(readingFifo, writingFifo, current, n, p, t);

      exit(0);
    }else{
      //printf("parent\t[pid %d]\n", getpid());
      //printf("parent\t\tpid: %d\n", getpid());
    }

    //sleep(2);
  
  }

  //sleep(2);


  //char fifo[20] = "";
  //snprintf(fifo, sizeof(fifo), "pipe%dto%d", 1, 2);
  //printf("%s\n", fifo);

  //int fd = open(fifo,O_WRONLY);
  //write(fd, "0", strlen("0"));
  //close(fd);







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

