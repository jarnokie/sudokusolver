#ifndef INT_LIST_H
#define INT_LIST_H


typedef struct IntList {
    int val;
    struct IntList * previous;
    struct IntList * next;
} IntList;


IntList * list_new(int const initial);
void list_free(IntList * list);
void list_add(IntList * const list, int const value);
int list_get(IntList const * const list, int const index);
int list_length(IntList const * const list);
IntList * list_del(IntList * list, int const index);
void list_print(IntList const * list);

#endif