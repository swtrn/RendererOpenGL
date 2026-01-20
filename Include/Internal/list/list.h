// XOR linked list

#include <stdio.h>

struct Node {
  struct Node *next;
  void *data;
} typedef Node;

struct List {
  Node *start;
  Node *end;
  size_t size;
} typedef List;

void ListAppend(List *list, void *data, size_t size);
void ListClear(List *list);
void ListPrint(List *list);
