#include "list.h"
#include "queue.h"
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define NUMTHREADS 128

pthread_t works[NUMTHREADS];
int works_num = 0;

int delta;
List *queue_list_head;

void *pub(void *arg){
    char *name = (char *)arg;
    FILE *pFile = fopen(name, "r");

    while(1){
        char cmd[32];
        fscanf(pFile, "%s", cmd);

        if(strcmp(cmd, "put") == 0){
            int id;
            char url[URLSIZE];
            char cap[CAPSIZE];
            fscanf(pFile, "%d", &id);
            fscanf(pFile, "%s", url);
            fscanf(pFile, "%s", cap);

            TopicEntry *entry = entry_init(id, url, cap);
            entry->pubID = (int)pthread_self();

            List *node = list_find(queue_list_head, id);
            if(!node){
                printf("node == NULL\n");
            }
            else{
                enqueue(node->href, entry);
            }

            printf("Proxy thread %d - type: Publisher - Executed command: %s %d %s %s <%p>\n", (int)pthread_self(), cmd, id, url, cap, node);
        }
        else if(strcmp(cmd, "sleep") == 0){
            int times;
            fscanf(pFile, "%d", &times);
            printf("Proxy thread %d - type: Publisher - Executed command: %s %d\n", (int)pthread_self(), cmd, times);
            sleep(times / 1000);
        }
        else{
            printf("Proxy thread %d - type: Publisher - Executed command: %s\n", (int)pthread_self(), cmd);
            break;
        }
    }

    //free
    fclose(pFile);
    free(name);
    return NULL;
}

void *sub(void *arg){
    char *name = (char *)arg;
    FILE *pFile = fopen(name, "r");

    char out_file[32];
    sprintf(out_file, "%ld.txt", pthread_self());
    FILE *pOut = fopen(out_file, "w");

    int last_num = 0;

    while(1){
        char cmd[32];
        fscanf(pFile, "%s", cmd);

        if(strcmp(cmd, "get") == 0){
            int id;
            fscanf(pFile, "%d", &id);

            List *node = list_find(queue_list_head, id);
            if(!node){
                printf("node == NULL\n");
                continue;
            }
            TopicEntry topicEntry;
            while((last_num = getentry(node->href, last_num, &topicEntry)) != 0) {
                fprintf(pOut, "%d %d %s %s\n",
                        topicEntry.entryNum, topicEntry.pubID, topicEntry.photoURL, topicEntry.photoCaption);
                last_num = topicEntry.entryNum + 1;
            }

            printf("Proxy thread %d - type: Subscriber - Executed command: %s %d\n", (int)pthread_self(), cmd, id);
        }
        else if(strcmp(cmd, "sleep") == 0){
            int times;
            fscanf(pFile, "%d", &times);
            printf("Proxy thread %d - type: Subscriber - Executed command: %s %d\n", (int)pthread_self(), cmd, times);
            sleep(times / 1000);
        }
        else{
            printf("Proxy thread %d - type: Subscriber - Executed command: %s\n", (int)pthread_self(), cmd);
            break;
        }
    }

    //free
    fclose(pOut);
    fclose(pFile);
    free(name);

    return NULL;
}

int main(int argc, char **argv){
    queue_list_head = list_init();

    while(1){
        char cmd[32];
        scanf("%s", cmd);
        if(strcmp(cmd, "create") == 0){
            scanf("%s", cmd);
            int id, length;
            char name[NAMESIZE];
            scanf("%d", &id);
            scanf("%s", name);
            scanf("%d", &length);

            for(int i = 0; i < strlen(name) - 1; ++ i){
                name[i] = name[i + 1];
            }
            name[strlen(name) - 2] = '\0';

            TopicQueue *queue = queue_init(id, length, name);
            list_insert(queue_list_head, queue);
        }
        else if(strcmp(cmd, "query") == 0){
            scanf("%s", cmd);

            List *list_curr = queue_list_head->next;
            while(list_curr != queue_list_head){
                printf("Topic id: %d\tTopic length: %d\n", list_curr->href->id, list_curr->href->length);
                list_curr = list_curr->next;
            }
        }
        else if(strcmp(cmd, "add") == 0){
            scanf("%s", cmd);

            char *file_name = (char *)malloc(sizeof(char) * 128);
            scanf("%s", file_name);

            for(int i = 0; i < strlen(file_name) - 1; ++ i){
                file_name[i] = file_name[i + 1];
            }
            file_name[strlen(file_name) - 2] = '\0';

            if(strcmp(cmd, "publisher") == 0){
                pthread_create(&works[works_num ++], NULL, pub, file_name);
            }
            else{
                pthread_create(&works[works_num ++], NULL, sub, file_name);
            }
        }
        else if(strcmp(cmd, "delta") == 0){
            int delta;
            scanf("%d", &delta);
            delta *= 1000000;
        }
        else{
            break;
        }
    }

    for(int i = 0; i < works_num; ++ i){
        pthread_join(works[i], NULL);
    }

    list_destroy(queue_list_head);
}
