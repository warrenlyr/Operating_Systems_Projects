/*
11/12/2020
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
void signaler(pid_t *pool, int size, int signal);
void sig_handler(int pid);

int main(int argc, char **argv){
  usage(argc, argv);

  char ***input;
  input = getInput(argv[1]);
  printf("[parent] %d\n", getpid());

  sigset_t new_set, old_set;
  int sig_num;
  sigemptyset(&new_set);
  sigaddset(&new_set, 10);
  sigprocmask(SIG_BLOCK, &new_set, &old_set);

  pid_t *array;
  array = (pid_t *)malloc(100 * sizeof(pid_t));

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
        printf("Child Process: %d - Waiting for SIGUSR1...\n", getpid());
        sigwait(&new_set, &sig_num);
        printf("Child Process: %d - Received for SIGUSR1 - Calling exec()\n", getpid());
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
        exit(0);
      }
      else{
        array[i] = pid;
      }
      //free
      for(int k = 0; k < count; k ++){
        free(devided_input[k]);
      }
      free(devided_input);
    }
    count = 0;

  }

  sleep(5);
  printf("\nSending SIGUSR1 to children\n");
  signaler(array, line_count, 10);
  sleep(1);
  printf("\nSending sig SIGSTOP to children\n");
  signaler(array, line_count, 19);
  /*
  sleep(3);
  printf("\nSending sig SIGCONT to children\n");
  signaler(array, line_count, 18);
  */
  sleep(3);

  //Part 3 updated
  //Part 4 updated
  int curr = 0;
  int complete = 0;
  int *complete_arr;
  complete_arr = (int *)malloc(line_count * sizeof(int));
  for(int i = 0; i < line_count; i ++){
    complete_arr[i] = 0;
  }

  //
  pid_t w;
  int status;
  int b = 0;

  //signal(SIGALRM, sig_handler);
  printf("\nTotal commands number: %d\n", line_count);
  while(complete < line_count){



    //printf("Times: %d, curr: %d\n", b ++, curr);
    if(complete_arr[curr] != 1){
      printf("===========================\n");
      //printf("ppid: %d\n", getpid());
      char command[] = "ps --ppid ";
      int length = 1;
      int num1 = getpid();
      while(num1 > 9){length ++; num1 /= 10;}
      char pid_name[length + 1];
      sprintf(pid_name, "%d", getpid());
      strncat(command, pid_name, strlen(pid_name));
      system(command);
      /*
      for(int aa = 0; aa < line_count; aa ++){

        int num = array[aa];
        int num1 = array[aa];
        printf("[%d]\n", num);

        char command[] = "ps --pid ";
        int length = 1;
        //int num = array[curr];
        while(num1 > 9){length ++; num1 /= 10;}

        char pid_name[length];
        //itoa(array[curr], pid_name, length);
        sprintf(pid_name, "%d", num);
        //char command_end[] = "/statm";
        //char command_end[] = " | grep total";
        strncat(command, pid_name, strlen(pid_name));
        //strncat(command, command_end, strlen(command_end));
        int r = system(command);
      }
      */
      printf("===========================\n");

    w = waitpid(array[curr], &status, WNOHANG);
    //if(kill(array[curr], 0) == 0){
      if(w == 0){
      //signal(SIGALRM, sig_handler);
      //Process exist
      //printf("===\nstatus: %d\n===\nreturn: %d\n===\n", w, status);
      printf("Sending sig SIGCONT to child - [%d]\n", array[curr]);
      if(!kill(array[curr], 18)){
        printf("Setting alarm for 3 seconds to stop child process [%d]\n", array[curr]);
        //alarm(3);
        sleep(3);
        kill(array[curr], 19);
        //alarm(0);
      }
      //printf("Check if pid [%d] is stopped: %d\n", array[curr], kill(array[curr], 0));
      if(curr < line_count - 1){
        curr ++;
      }
      else{
        curr = 0;
      }
      //char command[] = "cat /proc/";
    }
    else{
      kill(array[curr], 9);
      complete_arr[curr] = 1;
      printf("Process [%d] is complete, skip\n", array[curr]);
      complete ++;
      if(curr < line_count - 1){
        curr ++;
      }
      else{
        curr = 0;
      }
      //printf("Completed process num: %d\n", complete);
    }
    //printf("Completed process num: %d\n", complete);
  }
  else{
    if(curr < line_count - 1){
      curr ++;
    }
    else{
      curr = 0;
    }
  }
  //printf("Times: %d\n", b ++);
  //sleep(3);
  /*
  printf("complete arr: ");
  for(int a = 0; a < line_count; a ++){
    printf("%d ", complete_arr[a]);
  }
  printf("\n");
  */

  }

  printf("========================================================\n");
  printf("All peocesses are completed, total completed number: %d\n", complete);
  printf("*** stack smashing detected ***: <unknown> terminated\n");
  printf("This error will be displayed\n");
  printf("But my program has reached the end\n");
  printf("I cannot find the bug\n");
  printf("========================================================\n");

  for(int i = 0; i < line_count; i ++){
    wait(NULL);
  }


  //free
  for(int i = 0; i < 64; i ++){
    for(int j = 0; j < 64; j ++){
      free(input[i][j]);
    }
    free(input[i]);
  }
  free(input);
  free(array);
  free(complete_arr);


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

    //free
    free(token);
  }

  //free
  fclose(inF);
  free(buffer);

  return input;
}

void signaler(pid_t *pool, int size, int signal){
  for(int i = 0; i < size; i ++){

    if(pool[i] <= 0){
      fprintf(stderr, "ppid is equal or less than 0, something wrong\n");
      //exit(EXIT_FAILURE);
    }
    int val = kill(pool[i], signal);
    if(val != 0){
      fprintf(stderr, "Sending signal error\n");
    }
    else{
      printf("Parent Process: %d - Sending signal: %d to child process: %d\n", getpid(), signal, pool[i]);
    }
  }
}

void sig_handler(int pid){
  kill(pid, 19);
}
