//
//  myfunc.h
//  
//
//  Created by Warren Liu on 10/16/20.
//

#ifndef myfunc_h
#define myfunc_h

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include "command.h"

void usage(int argc, char **argv);
char **getInput();
char **getInput_FileMode(char *fileName);
void devideCommand(char **, int);
int case_check(char **, int);
void clean(char **);

#endif /* myfunc_h */
