#include "entry.h"
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>

TopicEntry *entry_init(int pubID, char *url, char *cap){
    TopicEntry *pnte = (TopicEntry *)malloc(sizeof(TopicEntry));

    strcpy(pnte->photoURL, url);
    strcpy(pnte->photoCaption, cap);
    gettimeofday(&pnte->timeStamp, NULL);
    //
    return pnte;
}

TopicEntry *entry_new(){
    TopicEntry *pnte = (TopicEntry *)malloc(sizeof(TopicEntry));
    //
    return pnte;
}

void entry_destroy(TopicEntry *pte)
{
    //free
    free(pte);
}

void entry_copy(TopicEntry *dest, TopicEntry *src)
{
    memcpy(dest, src, sizeof(TopicEntry));
}
