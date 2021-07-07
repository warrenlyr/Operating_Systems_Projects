#include "queue.h"
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

extern int delta;

TopicQueue *queue_init(int id, int length, char *topicName){
    //malloc
    TopicQueue *pntq = (TopicQueue *)malloc(sizeof(TopicQueue));

    pntq->mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(pntq->mutex, NULL);
    pntq->cond  = (pthread_cond_t  *)malloc(sizeof(pthread_cond_t));
    pthread_cond_init(pntq->cond, NULL);

    //Initial
    pntq->id = id;
    strcpy(pntq->name, topicName);
    pntq->length = length;

    pntq->count = 0;
    pntq->last_count = 0;
    pntq->queue = (TopicEntry **)malloc(sizeof(TopicEntry *) * length);
    pntq->head = pntq->queue;
    pntq->tail = pntq->queue;

    return pntq;
}

void queue_destroy(TopicQueue *ptq){
    pthread_cond_destroy(ptq->cond);
    pthread_mutex_destroy(ptq->mutex);
    free(ptq->mutex);
    free(ptq->cond);

    for(int i = 0; i < ptq->count; ++ i){
        int loc = (ptq->head - ptq->queue + i) % ptq->length;
        free(ptq->queue[loc]);
    }

    free(ptq->queue);
    free(ptq);
}

void enqueue(TopicQueue *ptq, TopicEntry *pte){
    pthread_mutex_lock(ptq->mutex);
    while(ptq->count >= ptq->length){
        pthread_cond_wait(ptq->cond, ptq->mutex);
    }

    int loc = ptq->tail - ptq->queue;
    if(ptq->count){
        loc = (loc + 1) % ptq->length;
    }
    ptq->tail = ptq->queue + loc;

    pte->entryNum = ptq->last_count + 1;
    *ptq->tail = pte;
    ptq->count ++;
    ptq->last_count ++;

    pthread_mutex_unlock(ptq->mutex);
}

int getentry(TopicQueue *ptq, int last_entry, TopicEntry *pte){
    pthread_mutex_lock(ptq->mutex);

    //1
    if(ptq->count == 0) {
        pthread_mutex_unlock(ptq->mutex);
        return 0;
    }

    //2
    last_entry ++;
    if((*ptq->head)->entryNum > last_entry){
        entry_copy(pte, *ptq->head);
        pthread_mutex_unlock(ptq->mutex);
        return pte->entryNum;
    }

    //3
    if((*ptq->tail)->entryNum < last_entry){
        pthread_mutex_unlock(ptq->mutex);
        return 0;
    }

    //4
    for(int i = 0; i < ptq->count; ++ i){
        int loc = (ptq->head - ptq->queue + i) % ptq->length;
        if(ptq->queue[loc]->entryNum == last_entry){
            entry_copy(pte, ptq->queue[loc]);
            pthread_mutex_unlock(ptq->mutex);
            return 1;
        }
    }

    pthread_mutex_unlock(ptq->mutex);
}

static float time_spend(struct timeval *ptv){
    struct timeval current;
    gettimeofday(&current, NULL);

    float time_ret = (current.tv_sec - ptv->tv_sec) * 1000000 + current.tv_usec - ptv->tv_usec;

    return time_ret;
}

void dequeue(TopicQueue *ptq){
    //lock
    pthread_mutex_lock(ptq->mutex);

    int count = 0;
    for(int i = 0; i < ptq->count; ++ i){
        int loc = (ptq->head - ptq->queue + i) % ptq->length;
        float time_sec = time_spend(&ptq->queue[loc]->timeStamp);
        if(time_sec < delta){
            break;
        }
        else{
            entry_destroy(ptq->queue[loc]);
            ++ count;
        }
    }

    int loc = (ptq->head - ptq->queue + count) % ptq->length;
    ptq->head = ptq->queue + loc;
    ptq->count -= count;

    if(count > 0){
        pthread_cond_signal(ptq->cond);
    }

    //Unlock
    pthread_mutex_unlock(ptq->mutex);
}
