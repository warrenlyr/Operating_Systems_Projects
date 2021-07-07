//
//  myfunc.c
//  
//
//  Created by Warren Liu on 10/16/20.
//

#include "myfunc.h"

/*
 Usage check func
 exit if run code is wrong
 else do nothing
 */
void usage(int argc, char **argv){
    //Interactive mode
    if(argc == 1){
        if(strcmp(argv[0], "./pseudo-shell") != 0){
            write(2, "usuage: ", strlen("usuage: "));
            write(2, argv[0], strlen(argv[0]));
            write(2, "\n./pseudo-shell for Interactive mode\n", strlen("\n./pseudo-shell for Interactive mode\n"));
            write(2, "or ./pseudo-shell -f <input file> for File mode\n", strlen("or ./pseudo-shell -f <input file> for File mode\n"));
            exit(EXIT_FAILURE);
        }
    }
    //File mode
    if(argc == 3){
        if(strcmp(argv[0], "./pseudo-shell") != 0 || strcmp(argv[1], "-f") != 0){
            write(2, "usuage: ", strlen("usuage: "));
            write(2, argv[0], strlen(argv[0]));
            write(2, "\n./pseudo-shell for Interactive mode\n", strlen("\n./pseudo-shell for Interactive mode\n"));
            write(2, "or ./pseudo-shell -f <input file> for File mode\n", strlen("or ./pseudo-shell -f <input file> for File mode\n"));
            exit(EXIT_FAILURE);
        }
    }
    //
    if(argc != 1 && argc != 3){
        write(2, "usuage: ", strlen("usuage: "));
        write(2, argv[0], strlen(argv[0]));
        write(2, "\n./pseudo-shell for Interactive mode\n", strlen("\n./pseudo-shell for Interactive mode\n"));
        write(2, "or ./pseudo-shell -f <input file> for File mode\n", strlen("or ./pseudo-shell -f <input file> for File mode\n"));
        exit(EXIT_FAILURE);
    }
}

/*
 Get input, and devide them by ;
 Return a list of string
 a list of command
 */
char **getInput(){
    char *buffer;
    size_t size = 1024;
    size_t characters;
    
    //Get all input first
    if((buffer = (char *)malloc(size * sizeof(char))) == NULL){
        perror("Error: Unable to allocate buffer in getInputInteractive\n");
        exit(EXIT_FAILURE);
    }
    printf(">>> ");
    characters = getline(&buffer, &size, stdin);
    /*
    printf("===get input: %zu char were read.===\n", characters);
    printf("===get input: Entered: %s===\n", buffer);
     */
    
    if(characters > 0 && strcmp(&buffer[characters - 1], "\n") == 0){
        strcpy(&buffer[characters - 1], "\0");
    }
    /*
    printf("===get input: Entered after: %s===\n", buffer);
     */
    
    //Tokenize input
    
    //If buffer = ezit, exit
    if(strcmp(buffer, "exit") == 0){
        /*
        char *exit;
        exit = (char *)malloc(strlen("exit") * sizeof(char));
        exit = "exit";
        return &exit;
         */
        free(buffer);
        exit(EXIT_SUCCESS);
    }
    //else
    int num = 0;
    char *token;
    const char devide[] = ";";
    const char newline[] = "\n";
    //char *buffercpy = buffer;
    token = strtok(buffer, devide);
    //printf("Buffer now: %s\n", buffer);
    
    char **input;
    input = (char **)malloc(50 * sizeof(char *));
    for(int i = 0; i < 50; i ++){
        input[i] = (char *)malloc(1024 * sizeof(char));
        strcpy(input[i], "");
        //printf("%d: %s\n", i, input[i]);
    }
    
    while(token != NULL){
        if(strcmp(newline, token) != 0){
            //input[num ++] = token;
            strcpy(input[num ++], token);
            //printf("%s ", token);
        }
        token = strtok(NULL, devide);
    }
    /*
    printf("===get input: num: %d===\n", num);
    
    for(int i = 0; i < num; i ++){
        printf("%d%s ", i, input[i]);
    }
    printf("\n");
     */

    //free
    free(buffer);
    free(token);
    return input;
}

char **getInput_FileMode(char *fileName){
    char *buffer;
    size_t size = 1024;
    size_t characters;
    
    //Get all input first
    if((buffer = (char *)malloc(size * sizeof(char))) == NULL){
        perror("Error: Unable to allocate buffer in getInputInteractive\n");
        exit(EXIT_FAILURE);
    }
    //printf(">>> ");
    
    FILE *f = fopen(fileName, "r");
    if(f == NULL){
        perror("Error: Unable to open file\n");
        exit(EXIT_FAILURE);
    }
    
    int num = 0;
    char **input;
    input = (char **)malloc(50 * sizeof(char *));
    for(int i = 0; i < 50; i ++){
        input[i] = (char *)malloc(1024 * sizeof(char));
        strcpy(input[i], "");
    }
    
    while((characters = getline(&buffer, &size, f)) != EOF){
    
    
        if(characters > 0 && strcmp(&buffer[characters - 1], "\n") == 0){
            strcpy(&buffer[characters - 1], "\0");
        }
    
        //Tokenize input
    
        //If buffer = ezit, exit
        if(strcmp(buffer, "exit") == 0){
            free(buffer);
            exit(EXIT_SUCCESS);
        }
        //else
    
        char *token;
        const char devide[] = ";";
        const char newline[] = "\n";
        token = strtok(buffer, devide);
    
        
    
        while(token != NULL){
            if(strcmp(newline, token) != 0){
                strcpy(input[num ++], token);
            }
            token = strtok(NULL, devide);
        }
        //
        free(token);
    }

    //free
    fclose(f);
    free(buffer);
    //free(token);
    return input;
}

/*
 Devide each input
 */
void devideCommand(char **input, int size){
    /*
    int num = 0;
    char **command;
    command = (char **)malloc(10 * sizeof(char *));
    for(int i = 0; i < 10; i ++){
        command[i] = (char *)malloc(100 * sizeof(char));
        strcpy(command[i], "");
    }*/
    
    for(int i = 0; i < size; i ++){
        
        int num = 0;
        char **command;
        command = (char **)malloc(50 * sizeof(char *));
        for(int i = 0; i < 50; i ++){
            command[i] = (char *)malloc(1024 * sizeof(char));
            strcpy(command[i], "");
        }
        
        char *token;
        const char devide[] = " ";
        token = strtok(input[i], devide);
        int token_num = 0;
        
        while(token != NULL){
            strcpy(command[num ++], token);
            token = strtok(NULL, devide);
            token_num ++;
        }
        /* TEST PART */
        /*
        printf("===divide: command arr%d:===\n", i);
        for(int j = 0; j < num; j ++){
            printf("%s ", command[j]);
        }
        printf("\n");
        printf("===divide: token num: %d===\n", token_num);
        printf("\n===Devide: new===\n");
         */
        /*============*/
        int returnValue = case_check(command, token_num);
        //printf("%d\n", returnValue);
        if(returnValue == -2){
            break;
        }
        if(returnValue == -1){
            fprintf(stderr, "Error! Incorrect syntax. No control code found.\n");
            break;
        }
        
        //free
        for(int i = 0; i < 50; i ++){
            free(command[i]);
        }
        free(command);
        free(token);
    }
}

int case_check(char **command, int token_num){
    if(strcmp(command[0], "ls") == 0){
        if(token_num != 1){
            fprintf(stderr, "Error! Unsupported parameters for command: ls\n");
            return -2;
        }
        listDir();
        return 1;
    }
    else if(strcmp(command[0], "pwd") == 0){
        if(token_num !=1){
            fprintf(stderr, "Error! Unsupported parameters for command: pwd\n");
            return -2;
        }
        showCurrentDir();
        return 1;
    }
    else if(strcmp(command[0], "mkdir") == 0){
        if(token_num != 2){
            fprintf(stderr, "Error! Unsupported parameters for command: mkdir\n");
            return -2;
        }
        char *dirName;
        dirName = (char *)malloc(100 * sizeof(char));
        strcpy(dirName, command[1]);
        makeDir(dirName);
        //need a free
        free(dirName);
        return 1;
    }
    else if(strcmp(command[0], "cd") == 0){
        if(token_num != 2){
            fprintf(stderr, "Error! Unsupported parameters for command: cd\n");
            return -2;
        }
        char *dirName;
        dirName = (char *)malloc(100 * sizeof(char));
        strcpy(dirName, command[1]);
        changeDir(dirName);
        //free
        free(dirName);
        return 1;
    }
    else if(strcmp(command[0], "cp") == 0){
        if(token_num != 3){
            fprintf(stderr, "Error! Unsupported parameters for command: cp\n");
            return -2;
        }
        char *sourcePath;
        char *destPath;
        sourcePath = (char *)malloc(100 * sizeof(char));
        destPath = (char *)malloc(100 * sizeof(char));
        strcpy(sourcePath, command[1]);
        strcpy(destPath, command[2]);
        copyFile(sourcePath, destPath);
        //free
        free(sourcePath);
        free(destPath);
        return 1;
    }
    else if(strcmp(command[0], "mv") == 0){
        if(token_num != 3){
            fprintf(stderr, "Error! Unsupported parameters for command: mv\n");
            return -2;
        }
        char *sourcePath;
        char *destPath;
        sourcePath = (char *)malloc(100 * sizeof(char));
        destPath = (char *)malloc(100 * sizeof(char));
        strcpy(sourcePath, command[1]);
        strcpy(destPath, command[2]);
        moveFile(sourcePath, destPath);
        //free
        free(sourcePath);
        free(destPath);
        return 1;
    }
    else if(strcmp(command[0], "rm") == 0){
        if(token_num != 2){
            fprintf(stderr, "Error! Unsupported parameters for command: rm\n");
            return -2;
        }
        char *fileName;
        fileName = (char *)malloc(100 * sizeof(char));
        strcpy(fileName, command[1]);
        deleteFile(fileName);
        //free
        free(fileName);
        return 1;
    }
    else if(strcmp(command[0], "cat") == 0){
        if(token_num != 2){
            fprintf(stderr, "Error! Unsupported parameters for command: cat\n");
            return -2;
        }
        char *fileName;
        fileName = (char *)malloc(100 * sizeof(char));
        strcpy(fileName, command[1]);
        displayFile(fileName);
        //free
        free(fileName);
        return 1;
    }
    //
    return -1;
}

//Func to free all memory
void clean(char **input){
    for(int i = 0; i < 50; i ++){
        free(input[i]);
    }
    free(input);
}
