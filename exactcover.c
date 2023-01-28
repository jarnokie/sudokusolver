#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "intlist.h"
#include "solver.h"

void copy_arr(bool const from[], bool to[], int const n)
{
  for (int i = 0; i < n; i++)
  {
    to[i] = from[i];
  }
}

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
 *
 * Solves the matrix 'mat' with the Knuth's Algorithm X. Selected rows are set to the 'selected' list.
 * To pre-select rows add them to the selected list and mark them removed in the rows array.
 *
 * @param mat Matrix to solve.
 * @param height Height of the matrix.
 * @param width Width of the matrix.
 * @param rows Removed rows.
 * @param cols Removed columns.
 * @param selected List of selected rows.
 * @returns True if matrix was solved, false otherwise.
 */
bool knuths_alg_x(bool **mat, int const height, int const width,
                  bool rows[], bool cols[], IntList *const selected)
{
  // Check for empty matrix = solved
  bool empty = true;
  for (int i = 0; i < height; i++)
  {
    if (!rows[i])
    {
      empty = false;
      break;
    }
  }
  if (!empty)
  {
    for (int i = 0; i < width; i++)
    {
      if (!cols[i])
      {
        empty = false;
        break;
      }
    }
  }
  if (empty)
    return true;

  // Find the columns with least selected values.
  IntList *col_min = list_new();
  int col_min_val = find_col_min(col_min, (const bool **)mat, height, width, rows, cols);

  // If one of the colums has no values = unsolvable
  if (col_min_val == 0)
    return false;

  // Start trying to solve with each of the selected columns.
  // Step 1 in the Wikipedia page algorithm.
  for (int col_index = 0; col_index < list_length(col_min); col_index++)
  {
    int const col = list_get(col_min, col_index);

    // Copy removed rows and cols so that they can be reset if solver path fails.
    bool rows_old[height];
    bool cols_old[width];
    copy_arr(rows, rows_old, height);
    copy_arr(cols, cols_old, width);

    // Find rows which have 1 set for the selected column
    IntList *row_indices = list_new();
    for (int row = 0; row < height; row++)
    {
      if (rows[row]) // Skip removed rows
        continue;
      if (mat[row][col])
        list_add(row_indices, row);
    }

    // Step 2 in the Wikipedia page algorithm.
    for (int i = 0; i < list_length(row_indices); i++)
    {
      int const row_index = list_get(row_indices, i);
      rows[row_index] = true;

      list_add(selected, row_index);

      // Remove columns of the selected row and the rows of the removed columns.
      for (int col = 0; col < width; col++)
      {
        if (cols[col])
          continue;
        if (mat[row_index][col])
        {
          // Wiki page step 4.
          cols[col] = true;
          for (int r = 0; r < height; r++)
          {
            if (mat[r][col])
            {
              rows[r] = true;
            }
          }
        }
      }

      // Wiki page step 5.
      bool const ret = knuths_alg_x(mat, height, width, rows, cols, selected);
      if (ret)
      {
        // Free before return.
        list_free(col_min);
        free(col_min);
        col_min = NULL;
        return true;
      }

      // Return selected, rows and cols as they were in the beginning of the loop
      list_del(selected, list_length(selected) - 1);
      copy_arr(rows_old, rows, height);
      copy_arr(cols_old, cols, width);
    }

    // Free created lists
    list_free(row_indices);
    free(row_indices);
    row_indices = NULL;
  }

  // Free before return.
  list_free(col_min);
  free(col_min);
  col_min = NULL;

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