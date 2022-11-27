#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>


//returns EXIT_FAILURE if snippet of text has a "\n"
int checkParagraph(char* str){
  for(int i = 0; i < strlen(str); i++){
    if(str[i] == '\n') return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}


int main(int argc, char* argv[]){

  time_t tt;
  srand((int)time(&tt) % getpid());

  FILE * fp;

  fp = fopen(argv[1], "r");
  if (fp == NULL)
      exit(EXIT_FAILURE);

  fseek(fp, 0, SEEK_END);

  size_t len = ftell(fp);
  
  if(len < atoi(argv[3])){
    printf("file smaller than n");
    return EXIT_FAILURE;
  }

  printf("file length: %d\n\n", len);

  char buffer[atoi(argv[3])];
  
  //printf("atoi(argv[3])= %d\n", atoi(argv[3]));

  //printf("buffer.size = %d\n", strlen(buffer));

  //char buffer = (char)malloc((atoi(argv[3])+1) * sizeof(char));

  fclose(fp);

  for(int i = 0; i < atoi(argv[2]); i++){
    
    fp = fopen(argv[1], "r");

    //printf("rand: %d\n", rand() % len);

    
    
    int offset = rand() % len;
    while(1){
      if((len - offset) < atoi(argv[3])) offset = rand() % len;
      else break;
    }

    fseek( fp, offset, SEEK_SET );

    fread(buffer, atoi(argv[3]), 1, fp);
    
    while(checkParagraph(buffer) == EXIT_FAILURE){

      offset = rand() % len;
      while(1){
	if((len - offset) < atoi(argv[3])) offset = rand() % len;
	else break;
      }
      fseek( fp, offset, SEEK_SET );
      fread(buffer, atoi(argv[3]), 1, fp);
    
    }
    
    //printf("buffer.size = %d\n", strlen(buffer));
    
    char result[atoi(argv[3])];

    printf(">");

    for(int i = 0; i < atoi(argv[3]); i++){
      printf("%c",buffer[i]);
    }
    printf("<\n");
    
    //printf(">%s<\n", result);
    
    fclose(fp);

  }


  //fclose(fp);

  exit(EXIT_SUCCESS);  
}

