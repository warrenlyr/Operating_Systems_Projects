#ifndef __X_ENTRY_H__
#define __X_ENTRY_H__

#include <sys/time.h>

#define URLSIZE 1024
#define CAPSIZE 512

typedef struct topicEntry {
    int entryNum;
    int pubID;
    char photoURL[URLSIZE];
    char photoCaption[CAPSIZE];
    struct timeval timeStamp;
}TopicEntry;

TopicEntry *entry_init(int, char *, char *);
TopicEntry *entry_new();
void entry_destroy(TopicEntry *);
void entry_copy(TopicEntry *, TopicEntry *);

#endif // __X_ENTRY_H__
