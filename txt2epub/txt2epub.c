#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

//claro que fui eu que fiz esta funcao 
char* substr(const char *src, int m, int n)
{
    // get the length of the destination string
    int len = n - m;
 
    // allocate (len + 1) chars for destination (+1 for extra null character)
    char *dest = (char*)malloc(sizeof(char) * (len + 1));
 
    // extracts characters between m'th and n'th index from source string
    // and copy them into the destination string
    for (int i = m; i < n && (*(src + i) != '\0'); i++)
    {
        *dest = *(src + i);
        dest++;
    }
 
    // null-terminate the destination string
    *dest = '\0';
 
    // return the destination string
    return dest - len;
}



int main(int argc, char* argv[]){

  printf("%d\n", argc);

  char* books[argc - 1];
  int zipCommandLength = strlen("zip ebooks.zip ");

  for(int i = 1; i < argc; i++){

    char* book = strcat(substr(argv[i], 0, strlen(argv[i]) - 4), ".epub");
    books[i - 1] = book;
    zipCommandLength += strlen(book) + 1;

    if(fork() == 0){

      char command[500];

      snprintf(command, sizeof(command), "pandoc %s -o %s >/dev/null 2>&1  &&  rm %s", argv[i], book, argv[i]);

      
      printf("[pid%d] converting %s...\n", getpid(), argv[i]);

      exit(system(command));
    }
  
  }



  /* Wait for children */
  
  int corpse;
  int status; // if stattus == 0 then great success 
  while ((corpse = wait(&status)) > 0){
	
    if(status == EXIT_FAILURE){
      printf("FAILLLLLLEDDDDDD");
      return EXIT_FAILURE;
    }
  }



  /*zip stuff*/

  char zipCommand[zipCommandLength];
  strcat(zipCommand, "zip ebooks.zip ");

  for(int i = 0; i < argc - 1;i++){
    strcat(zipCommand, strcat(books[i], " "));
  }

  printf("\nzipping files...\n");
  system(zipCommand);

  return EXIT_SUCCESS;

}


