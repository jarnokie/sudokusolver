#include <stdlib.h>
#include <stdio.h>
#include "intlist.h"

int list_length(IntList const * const list) {
    IntList iter = *list;
    int count = 1;
    while (iter.next != NULL) {
        iter = *iter.next;
        count++;
    }
    return count;
}


IntList * get_last(IntList * list) {
    IntList * iter = list;
    while (iter->next != NULL) {
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
    IntList * last = get_last(list);
    IntList * new = list_new(value);
    last->next = new;
    new->previous = last;
}


IntList * list_new(int const initial) {
    IntList * new = malloc(sizeof(IntList));
    new->next = NULL;
    new->previous = NULL;
    new->val = initial;
    return new;
}


void list_free(IntList * list) {
    IntList * iter;
    IntList * next = list;
    do {
        iter = next;
        next = iter->next;
        free(iter);
    } while(next != NULL);
}


IntList * list_get_iter(IntList const * list, int const index) {
    int i = index;
    IntList const * iter = list;
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
    IntList * iter = list_get_iter(list, index);
    return iter == NULL ? 0 : iter->val;
}


/**
 * Remove selected index from the list.
 *
 * @param index Index to remove from the list
 * @param list Pointer to the list 
 * @returns Pointer to the list, null if list empty
 */
IntList * list_del(IntList * list, int const index) {
    if (index > 0 && list->next != NULL) {
        list_del(list->next, index - 1);
        return list;
    }
    if (list->next != NULL) {
        list->val = list->next->val;
        list->next = list_del(list->next, 0);
        return list;
    }
    free(list);
    return NULL;
}

void list_print(IntList const * list) {
    IntList const * iter = list;
    printf("[");
    char sep = ' ';
    while(iter != NULL) {
        printf("%c %d", sep, iter->val);
        iter = iter->next;
        sep = ',';
    }
    printf(" ]\n");
}

