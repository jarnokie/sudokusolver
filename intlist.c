#include <stdlib.h>
#include <stdio.h>
#include "intlist.h"

int list_length(IntList const * const list) {
    IntListIter * iter = list->first;
    int count = 0;
    while (iter != NULL) {
        iter = iter->next;
        count++;
    }
    return count;
}


IntListIter * get_last(IntList * list) {
    IntListIter * iter = list->first;
    while (iter != NULL) {
        if (iter->next == NULL) return iter;
        iter = iter->next;
    }
    return iter;
}


/**
 * Add number to the end of the list
 * @param list Pointer to the list
 * @param value Value to add to the end of the list
 */
void list_add(IntList * const list, int const value) {
    IntListIter * last = get_last(list);

    IntListIter * new = malloc(sizeof(IntListIter));
    new->previous = last;
    new->next = NULL;
    new->val = value;

    if (last != NULL) {
        last->next = new;
    } else {
        list->first = new;
    }
}


IntList * list_new() {
    IntList * new = malloc(sizeof(IntList));
    list_init(new);
    return new;
}


void list_init(IntList * list) {
    list->first = NULL;
}


/**
 * Frees all of the list items.
 * 
 * @param list Pointer to the list to be freed;
 */
void list_free(IntList * list) {
    IntListIter * iter;
    IntListIter * next = list->first;
    while(next != NULL) {
        iter = next;
        next = iter->next;
        free(iter);
    };
    list->first = NULL;
}


IntListIter * list_get_iter(IntList const * list, int const index) {
    int i = index;
    IntListIter const * iter = list->first;
    do {
        if (iter == NULL) {
            return NULL;
        }
        if (i <= 0) {
            return iter;
        }
        i--;
        iter = iter->next;
    } while(i >= 0);
    return NULL;
}


/**
 * Get the selected index from the list.
 * 
 * @param list Pointer to the list
 * @param index List index to get
 * @returns Index to get, 0 if invalid index
 */
int list_get(IntList const * list, int const index) {
    IntListIter * iter = list_get_iter(list, index);
    return iter == NULL ? 0 : iter->val;
}


IntListIter * list_del_iter(IntListIter * iter, int const index) {
    if (iter == NULL) return NULL;
    if (index > 0) {
        list_del_iter(iter->next, index - 1);
        return iter;
    }
    IntListIter * previous = iter->previous;
    IntListIter * next = iter->next;
    if (previous != NULL) {
        previous->next = next;
        if (next != NULL) next->previous = previous;
    } else if (next != NULL) {
        next->previous = previous;
    }
}


/**
 * Remove selected index from the list.
 *
 * @param index Index to remove from the list
 * @param list Pointer to the list 
 * @returns Pointer to the list, null if list empty
 */
void list_del(IntList * list, int const index) {
    list->first = list_del_iter(list->first, index);
}


void list_print(IntList const * list) {
    IntListIter const * iter = list->first;
    printf("[");
    char sep = ' ';
    while(iter != NULL) {
        printf("%c %d", sep, iter->val);
        iter = iter->next;
        sep = ',';
    }
    printf(" ]\n");
}

