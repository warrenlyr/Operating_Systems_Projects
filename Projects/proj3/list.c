#include "list.h"
#include <assert.h>

List *list_init(){
  List *new_list = NULL;
  new_list = (List *)malloc(sizeof(List));

  new_list->prev = new_list;
  new_list->next = new_list;

  return new_list;
}

List *list_peek(List *list_head){
  List *item_to_peek;
  item_to_peek = list_head->next;
  list_head->next = item_to_peek->next;
  list_head->next->prev = list_head;

  return item_to_peek;
}

int list_empty(List *list_head){
  if(list_head->next == list_head){
    return TRUE;
  }
  return FALSE;
}

int list_insert(List *list_head, TopicQueue *ptq){
  List *new_item = NULL;
  new_item = (List *)malloc(sizeof(List));
  new_item->href = ptq;

  new_item->next = list_head;
  new_item->prev = list_head->prev;
  list_head->prev->next = new_item;
  list_head->prev = new_item;

  return TRUE;
}

int list_insert_struct(List *list_head, List *to_insert){
  to_insert->next = list_head;
  to_insert->prev = list_head->prev;
  list_head->prev->next = to_insert;
  list_head->prev = to_insert;
  return TRUE;
}

void list_destroy(List *list_head){
  List *next;
  List *curr;
  next = list_head->next;

  while(next != list_head){
    curr = next;
    next = next->next;
    queue_destroy(curr->href);
    free(curr);
  }

  free(list_head);
}

List *list_find(List *list_head, int id){
  List *curr = list_head->next;
  while(curr != list_head){
    if(curr->href->id == id){
      return curr;
    }
    curr = curr->next;
  }
  return NULL;
}
