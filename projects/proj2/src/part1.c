/*
11/9/2020
Yiran Liu
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

char ***getInput(char*);
void usage(int, char **);

int main(int argc, char **argv){
  usage(argc, argv);
  char ***input;
  input = getInput(argv[1]);
  printf("[parent] %d\n", getpid());

  int count = 0;
  int line_count = 0;
  for(int i = 0; i < 64; i ++){
    for(int j = 0; j < 64; j ++){
      if(strcmp(input[i][j], "") != 0){
        count ++;
      }

    }
    if(count != 0){
      line_count ++;
      char **devided_input;
      devided_input = (char **)malloc((count + 1) * sizeof(char *));
      for(int k = 0; k < count; k ++){
        devided_input[k] = (char *)malloc(128 * sizeof(char));
      }
      if(devided_input == NULL){
        fprintf(stderr, "Error, mallocate memory in main-1\n");
      }
      for(int l = 0; l < count; l ++){
        strcpy(devided_input[l], input[i][l]);
        //printf("%s ", devided_input[l]);
      }
      //printf("\n");
      devided_input[count] = NULL;
      pid_t pid = fork();
      if(pid < 0){
        fprintf(stderr, "Error: fork failed in main\n");
        exit(0);
      }
      if(pid == 0){
        //printf("[son] %d - [parent] %d\n", getpid(), getppid());
        int val = 0;
        val = execvp(devided_input[0], devided_input);
        if(val){
          fprintf(stderr, "Error: cannot execute command at [son] %d\n", getpid());
          printf("Command entered is: ");
          for(int m = 0; m < count; m ++){
            printf("%s ", devided_input[m]);
          }
          printf("\n");
        }
        //printf("Process: %d - Finished.\n", getpid());
        exit(0);
      }
      //free
      for(int k = 0; k < count; k ++){
        free(devided_input[k]);
      }
      free(devided_input);
    }
    count = 0;

  }
  for(int i = 0; i < line_count; i ++){
    wait(0);
  }

  //free
  for(int i = 0; i < 64; i ++){
    for(int j = 0; j < 64; j ++){
      free(input[i][j]);
    }
    free(input[i]);
  }
  free(input);

  return EXIT_SUCCESS;
}

void usage(int argc, char **argv){
  if(argc != 2){
    fprintf(stderr, "Usage: ./<excutable program name> <input file>\n");
    exit(EXIT_FAILURE);
  }
  else{
    FILE *f = fopen(argv[1], "r");
    if(f == NULL){
      fprintf(stderr, "Input file is not found\n");
      exit(EXIT_FAILURE);
    }
    fclose(f);
  }
}

char ***getInput(char *inFile){
  char *buffer;
  size_t size = 1024;
  size_t characters;

  buffer = (char *)malloc(size * sizeof(char));
  if(buffer == NULL){
    fprintf(stderr, "Error: cannot allocate memory for buffer while reading from input file\n");
    exit(EXIT_FAILURE);
  }

  FILE *inF = fopen(inFile, "r");
  if(inF == NULL){
    fprintf(stderr, "Error: cannot open entered file while reading from input file\n");
    exit(EXIT_FAILURE);
  }

  char ***input;
  input = (char ***)malloc(64 * sizeof(char **));
  for(int i = 0; i < 64; i ++){
    input[i] = (char **)malloc(64 * sizeof(char *));
    for(int j = 0; j < 64; j ++){
      input[i][j] = (char *)malloc(128 * sizeof(char));
      strcpy(input[i][j], "");
    }
  }

  int line_num = 0;
  while((characters = getline(&buffer, &size, inF)) != EOF){
    //Get rid of newline
    if(characters > 0 && strcmp(&buffer[characters - 1], "\n") == 0){
        strcpy(&buffer[characters - 1], "\0");
    }

    //Tokenize buffer
    char *token;
    const char devide[] = " ";
    const char newline[] = "\n";
    token = strtok(buffer, devide);

    int token_num = 0;
    while(token != NULL){
      if(strcmp(newline, token) != 0){
        strcpy(input[line_num][token_num ++], token);
      }
      token = strtok(NULL, devide);
    }

    token_num = 0;
    line_num ++;
    //TEST
    /*
    printf("Line: %d\n", line_num);
    for(int i = 0; i < 10 ; i ++){
      printf("position: %d: %s\n", i, input[line_num - 1][i]);
    }
    printf("\n");
    */
    //TEST
    //free
    free(token);
  }

  //free
  fclose(inF);
  free(buffer);

  return input;
}
