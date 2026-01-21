#include "./list.h"

#include <stdlib.h>
#include <string.h>

void NewList(List *list) { list->size = 0; };

void ListAppend(List *list, void *data, size_t size) {
  Node *newNode = (Node *)malloc(sizeof(Node));
  if (!newNode) {
    printf("Failed allocating memory to node.");
    return;
  };

  // Assigning node values
  newNode->data = malloc(size);
  if (!newNode->data) {
    printf("Failed allocating memory to node data.");
    return;
  };

  memcpy(newNode->data, data, size);
  list->size++;

  // First node
  if (!list->start) {
    list->start = newNode;
    list->end = newNode;
    return;
  };

  list->end->next = newNode;
  list->end = newNode;
};

void ListClear(List *list) {
  // Clearing nodes
  for (Node *cur = list->start, *next; cur; cur = next) {
    next = cur->next;
    free(cur->data);
    free(cur);
  };

  list->start = NULL;
  list->end = NULL;
  list->size = 0;
};

void **ListToArray(List *list) {
  // Allocating memory for pointer array
  void **arr = (void **)malloc(list->size * sizeof(void *));
  if (!arr) {
    printf("Failed to allocate memory when creating array.");
    return NULL;
  };

  // Assigning data from list to array
  int i = 0;
  for (Node *cur = list->start; cur; cur = cur->next)
    arr[i++] = cur->data;

  // Returning pointer array of pointers
  return arr;
};

// TODO: Add Get function
