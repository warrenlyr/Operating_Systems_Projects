#ifndef __X_QUEUE_H__
#define __X_QUEUE_H__

#include "entry.h"
#include <pthread.h>

#define MAXTOPICS 32
#define NAMESIZE 64
#define DELTA (3 * 60 * 1000 * 1000)

typedef struct topicQueue{
    pthread_mutex_t *mutex;
    pthread_cond_t *cond;

    int id;
    char name[NAMESIZE];
    int length;

    int count;
    int last_count;
    struct topicEntry **head;
    struct topicEntry **tail;
    struct topicEntry **queue;
}TopicQueue;

TopicQueue *queue_init(int, int, char *);
void queue_destroy(TopicQueue *);

void enqueue(TopicQueue *, TopicEntry *);
int getentry(TopicQueue *, int, TopicEntry *);
void dequeue(TopicQueue *);

#endif // __X_QUEUE_H__
