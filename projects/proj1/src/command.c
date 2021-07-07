//
//  command.c
//  
//
//  Created by Warren Liu on 10/16/20.
//

#include "command.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>

void listDir(){
    char *dir;
    if((dir = (char *)malloc(1024 * sizeof(char))) == NULL){
        write(2, "Error! Cannot mallocate memory in showCurrentDir!\n", strlen("Error! Cannot mallocate memory in showCurrentDir!\n"));
        //exit(EXIT_FAILURE);
    }
    else{
        if((getcwd(dir, 1024)) == NULL){
            write(2, "Error: Cannot reading current path!\n", strlen("Error: Cannot reading current path!\n"));
            free(dir);
            //exit(EXIT_FAILURE);
        }
        else{
    
            DIR *dirPtr;
            //dirPtr = opendir(dir);
            struct dirent *filePtr;
            if((dirPtr = opendir(dir)) == NULL){
                write(2, "Error: Cannot open directory!\n", strlen("Error: Cannot open directory!\n"));
                free(dir);
                //exit(EXIT_FAILURE);
            }
            else{
    
                while((filePtr = readdir(dirPtr)) != NULL){
                    if(strcmp(filePtr->d_name, ".") != 0 && strcmp(filePtr->d_name, "..")){
                        write(1, filePtr->d_name, strlen(filePtr->d_name));
                        write(1, "\n", strlen("\n"));
                    }
                }
            
    //printf("I AM HERE LS\n");
    //free
                free(dir);
    //dirPtr = NULL;
    //printf("I AM HERE LS 1\n");
    //free(ptr);
    //printf("I AM HERE LS 2\n");
                closedir(dirPtr);
            }
        }
    }
}

void showCurrentDir(){
    char *dir;
    if((dir = (char *)malloc(1024 * sizeof(char))) == NULL){
        write(2, "Error! Cannot mallocate memory in showCurrentDir!\n", strlen("Error! Cannot mallocate memory in showCurrentDir!\n"));
        exit(EXIT_FAILURE);
    }
    else{
        char *ptr;
        if((ptr = getcwd(dir, 1024)) == NULL){
            write(2, "Error: Cannot reading current path!\n", strlen("Error: Cannot reading current path!\n"));
            free(dir);
            //exit(EXIT_FAILURE);
        }
        else{
    //printf("%s\n", ptr);
            write(1, ptr, strlen(ptr));
            write(1, "\n", strlen("\n"));
    //free
            free(dir);
        }
    }
}

void makeDir(char *dirName){
    /*
     I CHECKED, mkdir in in the syscalls page
     */
    int check = mkdir(dirName, 0777);
    if(check){
        write(2, "Error! Cannot create directory\n", strlen("Error!, Cannot create directory\n"));
        //exit(EXIT_FAILURE);
    }
}

void changeDir(char *dirName){
    /*
     I CHECKED, chdir is in the syscalls page
     */
    int check = chdir(dirName);
    if(check){
        write(2, "Error! Cannot change directory\n", strlen("Error!, Cannot change directory\n"));
        //exit(EXIT_FAILURE)
    }
}

void copyFile(char *sourcePath, char *destinationPath){
    DIR *src_dirPtr;
    src_dirPtr = opendir(sourcePath);
    if(src_dirPtr == NULL){
        write(2, "Error! Cannot open directory\n", strlen("Error!, Cannot open directory\n"));
        //
        closedir(src_dirPtr);
        //exit(EXIT_FAILURE);
    }
    else{
        struct dirent *src_filePtr;
        while((src_filePtr = readdir(src_dirPtr)) != NULL){
            if(strcmp(src_filePtr->d_name, ".") != 0 && strcmp(src_filePtr->d_name, "..") != 0 && strcmp(src_filePtr->d_name, ".DS_Store") != 0){
            //TEST
            /*
            write(1, src_filePtr->d_name, strlen(src_filePtr->d_name));
            write(1, "\n", strlen("\n"));
            
            //chdir(destinationPath);
            //printf("%s\n", destinationPath);
            DIR *dst_dirPtr;
            dst_dirPtr = opendir(destinationPath);
            struct dirent *dst_filePtr;
            while((dst_filePtr = readdir(dst_dirPtr)) != NULL){
                if(strcmp(dst_filePtr->d_name, ".") != 0 && strcmp(dst_filePtr->d_name, "..") != 0 && strcmp(dst_filePtr->d_name, ".DS_Store") != 0){
                    //TEST
                    write(1, dst_filePtr->d_name, strlen(dst_filePtr->d_name));
                    write(1, "\n", strlen("\n"));
                }
            }
            */
                chdir(sourcePath);
                FILE *src_file;
                src_file = fopen(src_filePtr->d_name, "r");
                /*
                 if(src_file == NULL){
                 write(2, "Error! Cannot open file\n", strlen("Error!, Cannot open file\n"));
                 exit(EXIT_FAILURE);
                 }*/
                char *buffer;
                size_t size = 1024;
                size_t read;
                buffer = (char *)malloc(size * sizeof(char));
                while((read = getline(&buffer, &size, src_file)) != EOF){
                    chdir(destinationPath);
                //FILE *dst_file = freopen(src_filePtr->d_name, "a", stdout);
                    int dst_file = open(src_filePtr->d_name, O_CREAT | O_WRONLY | O_APPEND, S_IRUSR | S_IWUSR | S_IXUSR);
                    write(dst_file, buffer, strlen(buffer));
                //write(1, "\n", strlen("\n"));
                //fclose(dst_file);
            }
            //free
                free(buffer);
                fclose(src_file);
            }
        }
        //free
        closedir(src_dirPtr);
    }
}

void moveFile(char *sourcePath, char *destinationPath){
    DIR *src_dirPtr;
    src_dirPtr = opendir(sourcePath);
    if(src_dirPtr == NULL){
        write(2, "Error! Cannot open directory\n", strlen("Error!, Cannot open directory\n"));
        closedir(src_dirPtr);
        //exit(EXIT_FAILURE);
    }
    else{
        struct dirent *src_filePtr;
        while((src_filePtr = readdir(src_dirPtr)) != NULL){
            if(strcmp(src_filePtr->d_name, ".") != 0 && strcmp(src_filePtr->d_name, "..") != 0 && strcmp(src_filePtr->d_name, ".DS_Store") != 0){
            
                chdir(sourcePath);
                FILE *src_file;
                src_file = fopen(src_filePtr->d_name, "r");
            //TEST
            /*
            if(src_file == NULL){
                write(2, "Error! Cannot open file\n", strlen("Error!, Cannot open file\n"));
                exit(EXIT_FAILURE);
            }*/
                char *buffer;
                size_t size = 1024;
                size_t read;
                buffer = (char *)malloc(size * sizeof(char));
                while((read = getline(&buffer, &size, src_file)) != EOF){
                    chdir(destinationPath);
                /*
                FILE *dst_file = freopen(src_filePtr->d_name, "a", stdout);
                write(1, buffer, strlen(buffer));
                fclose(dst_file);
                 */
                    int dst_file = open(src_filePtr->d_name, O_CREAT | O_WRONLY | O_APPEND, S_IRUSR | S_IWUSR | S_IXUSR);
                    write(dst_file, buffer, strlen(buffer));
                }
            //free
                free(buffer);
                fclose(src_file);
                chdir(sourcePath);
                int check = unlink(src_filePtr->d_name);
                if(check){
                    write(2, "Error! Cannot unlink file\n", strlen("Error!, Cannot unlink file\n"));
                    //exit(EXIT_FAILURE);
                }
            }
        }
        //free
        closedir(src_dirPtr);
    }
    
}

void deleteFile(char *filename){
    int check = unlink(filename);
    if(check){
        write(2, "Error! Cannot unlink file\n", strlen("Error!, Cannot unlink file\n"));
        //exit(EXIT_FAILURE);
    }
}

void displayFile(char *filename){
    FILE *f = fopen(filename, "r");
    if(f == NULL){
        write(2, "Error! Cannot open file\n", strlen("Error!, Cannot open file\n"));
        fclose(f);
        //exit(EXIT_FAILURE);
    }
    else{
        char *buffer;
        size_t size = 1024;
        size_t read;
        buffer = (char *)malloc(size * sizeof(char));
        while((read = getline(&buffer, &size, f)) != EOF){
            write(1, buffer, strlen(buffer));
        }
        write(1, "\n", strlen("\n"));
        //free
        fclose(f);
        free(buffer);
    }
}
