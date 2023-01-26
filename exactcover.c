#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "intlist.h"
#include "solver.h"

void matrix_row_to_row_col_n(int const m_row, int *row, int *col, int *n)
{
  *row = m_row / 81;
  *col = (m_row % 81) / 9;
  *n = (m_row % 9) + 1;
}

void init_ec_matrix(bool mat[729][324])
{
  for (int row = 0; row < 9; row++)
  {
    for (int col = 0; col < 9; col++)
    {
      int const box = col / 3 + (row / 3) * 3;
      for (int n = 0; n < 9; n++)
      {
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
 * Find the columns with the least set values from the bool matrix 'mat'. List 'max_indices' is first cleared
 * and column indices with min values are added to the list.
 *
 * @param min_indexes List to fill with the min column indices
 * @param mat Matrix to check values from
 * @param height Matrix height
 * @param width Matrix width
 * @param rows Disabled rows from the matrix
 * @param cols Disabled cols from the matrix
 * @returns What the minimum number of true bools was
 */
int find_col_min(IntList *const min_indices, bool const *const *const mat, int const height, int const width,
                 bool const rows[], bool const cols[])
{
  list_free(min_indices);
  int min = height;

  // Find the columns with most values
  for (int col = 0; col < width; col++)
  {
    if (cols[col])
      continue;
    int col_values = 0;
    for (int row = 0; row < height; row++)
    {
      if (rows[row])
        continue;
      if (mat[row][col])
        col_values++;
    }
    if (col_values < min)
    {
      list_free(min_indices);
      min = col_values;
      list_add(min_indices, col);
    }
    else if (col_values == min)
    {
      list_add(min_indices, col);
    }
  }

  return min;
}

void print_mat(bool const **mat, int const height, int const width, bool const rows[], bool const cols[])
{
  printf("  ");
  for (int col = 0; col < width; col++)
  {
    if (cols[col])
      continue;
    printf("%d ", col);
  }
  printf("\n");
  for (int row = 0; row < height; row++)
  {
    if (rows[row])
      continue;
    printf("%d ", row);
    for (int col = 0; col < width; col++)
    {
      if (cols[col])
        continue;
      printf("%d ", mat[row][col]);
    }
    printf("\n");
  }
}

/**
 * https://en.wikipedia.org/wiki/Knuth%27s_Algorithm_X
 */
bool knuths_alg_x(bool **mat, int const height, int const width,
                  bool rows[], bool cols[], IntList *const selected)
{

  printf("current mat:\n");
  print_mat((const bool **)mat, height, width, rows, cols);

  IntList *const col_min = list_new();
  // This should look for min...
  int col_min_val = find_col_min(col_min, (const bool **)mat, height, width, rows, cols);

  printf("min: ");
  list_print(col_min);

  if (col_min_val == 0)
  {
    printf("Column min = 0\n");
    return false;
  }

  for (int col_index = 0; col_index < list_length(col_min); col_index++)
  {
    int const col = list_get(col_min, col_index);
    printf("Selected col: %d\n", col);

    // Find rows which have 1 set for the selected column
    IntList *row_indices = list_new();
    for (int row = 0; row < height; row++)
    {
      if (rows[row])
        continue;
      if (mat[row][col])
        list_add(row_indices, row);
    }
    // TODO select non-determinastically
    int const row_index = list_get(row_indices, 0);
    printf("Selecting row: %d\n", row_index);
    list_add(selected, row_index);

    // Remove columns that the row has set
    for (int col = 0; col < width; col++)
    {
      if (cols[col])
        continue;
      if (mat[row_index][col])
      {
        printf("Removing column %d\n", col);
        cols[col] = true;
        for (int r = 0; r < height; r++)
        {
          if (mat[r][col])
          {
            if (!rows[r])
              printf("Removing row %d\n", r);
            rows[r] = true;
          }
        }
      }
    }

    // Free created lists
    list_free(row_indices);
    free(row_indices);
    row_indices = NULL;

    bool const ret = knuths_alg_x(mat, height, width, rows, cols, selected);
    if (ret)
      return true;

    // TODO: return selected, rows and cols as they were in the beginning of the col loop
    return false;
  }

  printf("current mat:\n");
  print_mat((const bool **)mat, height, width, rows, cols);

  return false;
}

/**
 * https://en.wikipedia.org/wiki/Exact_cover#Sudoku
 */
bool exact_cover(Sudoku *const sudoku)
{

  if (!is_valid(sudoku))
  {
    return false;
  }

  bool m[729][324];
  init_ec_matrix(m);

  IntList *locked = list_new();

  // TODO

  list_free(locked);
  locked = NULL;

  return false;
}