//
//  main.c
//  
//
//  Created by Warren Liu on 10/16/20.
//

/*
 I don't really understand the cp/mv functions
 in my understanding
 I'm suppose to copy/move all files from <src> dir to <dst> dir
 Because the discription is:
    cp: Only for copying files. Copy one file from the path specified by <src> to the location specified by <dst>.
    mv: Only for moving files(cut-paste). Move a file from the location specified by <src> to the location specified by <dst>.
 So what I did is [cp <src dir> <dst dir>] and copy all file in the src to dst
 Same as mv function
 
 */

#include <stdio.h>
#include "myfunc.h"

int main(int argc, char **argv){
    //Usage check first
    /*
    usage(argc, argv);
    char **input;
     */
    /*
    input = getInput();
    if(strcmp("exit", input[0]) == 0){
        printf("exit detected\n");
        exit(EXIT_SUCCESS);
    }
    
    int size = 0;
    //printf("===Main returned input===\n");
    while(strcmp(input[size], "") != 0){
        //printf("%d%s ", size, input[size]);
        size ++;
    }
    //printf("\n===main: size: %d===\n", size);
    devideCommand(input, size);
    //printf("===new===\n");
     */
    /*
    int size = 0;
    input = getInput_FileMode(argv[2]);
    while(strcmp(input[size], "") != 0){
        printf("%d%s ", size, input[size]);
        size ++;
    }
    printf("\n===main: size: %d===\n", size);
    devideCommand(input, size);
    printf("===new===\n");
    */
    /*
    char **input1;
    input1 = (char **)malloc(5 * sizeof(char *));
    for(int i = 0; i < 5; i ++){
        input1[i] = (char *)malloc(100 * sizeof(char));
        input1[i] = "aaa";
    }
    for(int i = 0; i < 5; i ++){
        printf("%s\n", input1[i]);
    }*/
    
    usage(argc, argv);
    
    if(argc == 1){
        //Interactive Mode
        while(1){
            char **input;
            int size = 0;
            input = getInput();
            while(strcmp(input[size], "") != 0){
                size ++;
            }
            devideCommand(input, size);
            clean(input);
            size = 0;
        }
        
    }
    else{
        //File Mode
        char **input;
        int size = 0;
        input = getInput_FileMode(argv[2]);
        FILE *f = freopen("output.txt", "w", stdout);
        while(strcmp(input[size], "") != 0){
            size ++;
        }
        devideCommand(input, size);
        clean(input);
        fclose(f);
    }

    
    return 0;
}
