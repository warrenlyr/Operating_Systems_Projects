#ifndef __LIST_H__
#define __LIST_H__

#include <string.h>
#include <stdlib.h>
#include "queue.h"

#define TRUE 1
#define FALSE 0

typedef struct list{
  TopicQueue *href;
  struct list *next;
  struct list *prev;
}List;

List *list_init();
List *list_peek(List *);
int list_empty(List *);
int list_insert(List *, TopicQueue *);
int list_insert_struct(List *, List *);
void list_destroy(List *);
List *list_find(List *, int id);

#endif // __LIST_H__
