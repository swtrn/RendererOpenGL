#include "./list.h"

#include <stdlib.h>
#include <string.h>

void ListAppend(List *list, void *data, size_t size) {
  Node *newNode = (Node *)malloc(sizeof(Node));
  if (!newNode) {
    printf("Failed allocating memory to node.");
    return;
  };

  // Assigning node values
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
    free(cur);
  };

  list->start = NULL;
  list->end = NULL;
  list->size = 0;
};
