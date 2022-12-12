#include <stdbool.h>
#include <stdio.h>
#include "intlist.h"
#include "solver.h"


void matrix_row_to_row_col_n(int const m_row, int * row, int * col, int * n) {
    *row = m_row / 81;
    *col = (m_row % 81) / 9;
    *n = (m_row % 9) + 1;
}


void init_ec_matrix(bool mat[729][324]) {
  for (int row = 0; row < 9; row++) {
    for (int col = 0; col < 9; col++) {
      int const box = col / 3 + (row / 3) * 3;
      for(int n = 0; n < 9; n++) {
        // row-column constraint
        mat[row * 81 + col * 9 + n][row * 9 + col] = true;
        // row-number constraint
        mat[row * 81 + col * 9 + n][81 + row * 9 + n] = true;
        // column-number constraint
        mat[row * 81 + col * 9 + n][162 + col * 9 + n] = true;
        // box-number constraint
        mat[row * 81 + col * 9 + n][243 + box * 9 + n] = true;
      }
    }
  }
}

/**
 * https://en.wikipedia.org/wiki/Exact_cover#Sudoku
 */
bool exact_cover(Sudoku * const sudoku) {

  if (!is_valid(sudoku)) {
    return false;
  }

  bool m[729][324];
  init_ec_matrix(m);

  IntList * locked = NULL;

  for (int row = 0; row < 9; row++) {
    for (int col = 0; col < 9; col++) {
      if (sudoku->locked[row][col]) {
        int const n = sudoku->grid[row][col];
        if (locked == NULL) {
          locked = list_new(row * 81 + col * 9 + n-1);
        } else {
          list_add(locked, row * 81 + col * 9 + n-1);
        }
      }
    }
  }

  list_print(locked);

  IntList const * iter = locked;
  while(iter != NULL) {
    int row, col, n;
    matrix_row_to_row_col_n(iter->val, &row, &col, &n);
    printf("%d,%d %d\n", row, col, n);

    iter = iter->next;
  }

  list_free(locked); locked = NULL;

  return false;
}