#ifndef INT_LIST_H
#define INT_LIST_H

#include <stdbool.h>

typedef struct IntListIter
{
    int val;
    struct IntListIter *previous;
    struct IntListIter *next;
} IntListIter;

typedef struct IntList
{
    struct IntListIter *first;
} IntList;

IntList *list_new();
void list_init(IntList *list);
void list_free(IntList *list);
void list_add(IntList *const list, int const value);
bool list_contains(IntList *const list, int const value);
int list_get(IntList *list, int const index);
int list_length(IntList const *const list);
void list_del(IntList *list, int const index);
void list_print(IntList const *list);

#ifdef UNIT_TEST
// Nothing for now
#endif

#endif