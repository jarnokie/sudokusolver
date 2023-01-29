#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "intlist.h"
#include "solver.h"
#include "exactcover.h"

void copy_arr(bool const from[], bool to[], int const n)
{
  for (int i = 0; i < n; i++)
  {
    to[i] = from[i];
  }
}

void remove_rows_cols(bool **mat, bool *rows, bool *cols, int const height, int const width, int const row)
{
  for (int c = 0; c < width; c++)
  {
    if (mat[row][c])
    {
      cols[c] = true;
      for (int r = 0; r < height; r++)
      {
        if (mat[r][c])
          rows[r] = true;
      }
    }
  }
}

void ec_row_to_row_col_n(int const mat_row, int *row, int *col, int *n)
{
  *n = (mat_row % 9) + 1;
  *col = (mat_row % 81) / 9;
  *row = mat_row / 81;
}

/**
 * Initializes the matrix 'mat' with the sudoku 9x9 constraints.
 * Each row represents a row-column-number combination -> 9 * 9 * 9 = 729 rows.
 * Each column represents a constraint like for example "there exists a number 6 is in the column 2.
 *
 * @param mat Empty constrain 729x324 constraint matrix.
 */
void init_ec_matrix(bool **mat)
{
  for (int row = 0; row < 9; row++)
  {
    for (int col = 0; col < 9; col++)
    {
      // Get the box number. 9 boxes from left to right, top to bottom.
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
int find_col_min(IntList *const min_indices, bool **mat, int const height, int const width,
                 bool const rows[], bool const cols[])
{
  list_free(min_indices);
  int min = height;

  // Find the columns with most values
  for (int col = 0; col < width; col++)
  {
    if (cols[col])
      continue;
    int col_values = height;
    for (int row = 0; row < height; row++)
    {
      if (rows[row] || !mat[row][col])
        col_values--;
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
  int col_min_val = find_col_min(col_min, mat, height, width, rows, cols);

  // If one of the colums has no values = unsolvable
  if (col_min_val == 0)
  {
    // Free before return.
    list_free(col_min);
    free(col_min);
    col_min = NULL;
    return false;
  }

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
      // Wiki page step 4.
      remove_rows_cols(mat, &rows[0], &cols[0], height, width, row_index);

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
 * Solves the given sudoku as an exact cover problem with the Knuth's Algorithm X.
 * See https://en.wikipedia.org/wiki/Exact_cover#Sudoku
 *
 * @param sudoku Sudoku to solve
 */
bool exact_cover(Sudoku *const sudoku)
{
  if (!is_valid(sudoku))
    return false;

  // Initialize the exact cover constraint matrix.
  bool **mat;
  mat = (bool **)malloc(sizeof(bool *) * 729);
  for (int i = 0; i < 729; i++)
  {
    mat[i] = (bool *)malloc(sizeof(bool) * 324);
    for (int j = 0; j < 324; j++)
      mat[i][j] = false;
  }
  init_ec_matrix(mat);

  // Initialize the removed rows arrays.
  bool rows[729];
  for (int i = 0; i < 729; i++)
    rows[i] = false;
  bool cols[324];
  for (int i = 0; i < 324; i++)
    cols[i] = false;

  // Remove the rows and colums representing the number already set in the sudoku.
  for (int col = 0; col < 9; col++)
  {
    for (int row = 0; row < 9; row++)
    {
      if (sudoku->locked[row][col])
      {
        // Get the matrix row for the row-column-number:
        int const mat_row = sudoku->grid[row][col] - 1 + col * 9 + row * 81;
        remove_rows_cols(mat, &rows[0], &cols[0], 729, 324, mat_row);
      }
    }
  }

  // Selected rows list.
  IntList *selected = list_new();

  // Solve the exact cover problem.
  bool success = knuths_alg_x(mat, 729, 324, rows, cols, selected);

  // Set the selected values to the sudoku grid.
  IntListIter *iter = selected->first;
  while (iter != NULL)
  {
    int row, col, n;
    ec_row_to_row_col_n(iter->val, &row, &col, &n);
    sudoku->grid[row][col] = n;
    iter = iter->next;
  }

  list_free(selected);
  selected = NULL;

  for (int i = 0; i < 729; i++)
    free(mat[i]);
  free(mat);
  mat = NULL;

  return success;
}