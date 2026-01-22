#include "./list.h"

#include <stdlib.h>
#include <string.h>

void NewList(List *list) { list->size = 0; };

// Adds a node to list
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

// Adds all nodes from target to dest
void ListInsert(List *dest, List *target) {
  for (Node *node = target->start; node; node = node->next)
    ListAppend(dest, &node->data, sizeof(node->data));
};

// Frees all nodes from list
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

// Creates an array from list
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
