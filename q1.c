#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int checkParagraph(char* str){
  for(int i = 0; i < strlen(str); i++){
    if(str[i] == '\n') return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int main(int argc, char* argv[]){
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

  for(int i = 0; i < atoi(argv[2]); i++){
    
    //printf("rand: %d\n", rand() % len);
    
    int offset = rand() % len;

    fseek( fp, offset, SEEK_SET );

    fread(buffer, atoi(argv[3]), 1, fp);
    
    while(checkParagraph(buffer) == 1){

      offset = rand() % len;
      fseek( fp, offset, SEEK_SET );
      fread(buffer, atoi(argv[3]), 1, fp);
    
    }
      
    printf(">%s<\n", buffer);
    
  }


  fclose(fp);

  exit(EXIT_SUCCESS);  
}

