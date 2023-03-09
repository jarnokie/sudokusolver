#include <stdio.h>
#include <stdbool.h>
#include "solver.h"
#include "intlist.h"

static int BITS[] = {0, 1, 2, 4, 8, 16, 32, 64, 128, 256};

static char CHARS[] = {'.', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

void initialize_sudoku(Sudoku *const sudoku)
{
  for (int row = 0; row < 9; row++)
  {
    for (int column = 0; column < 9; column++)
    {
      sudoku->grid[row][column] = 0;
      sudoku->locked[row][column] = false;
    }
  }
}

char pos_to_char(Sudoku const *const Sudoku, int const row, int const col)
{
  return CHARS[Sudoku->grid[row][col]];
}

void print_sudoku(Sudoku const *const sudoku)
{
  for (int row = 0; row < 9; row++)
  {
    if (row % 3 == 0)
      printf("+-------+-------+-------+\n");

    printf("| %c %c %c | %c %c %c | %c %c %c |\n",
           pos_to_char(sudoku, row, 0), pos_to_char(sudoku, row, 1), pos_to_char(sudoku, row, 2),
           pos_to_char(sudoku, row, 3), pos_to_char(sudoku, row, 4), pos_to_char(sudoku, row, 5),
           pos_to_char(sudoku, row, 6), pos_to_char(sudoku, row, 7), pos_to_char(sudoku, row, 8));
  }
  printf("+-------+-------+-------+\n");
}

int column_available(Sudoku const *const sudoku, const int col)
{
  int result = 0;
  for (int i = 0; i < 9; i++)
  {
    int const val = sudoku->grid[i][col];
    result = result | BITS[val];
  }
  return (~result) & 511;
}

int row_available(Sudoku const *const sudoku, const int row)
{
  int result = 0;
  for (int i = 0; i < 9; i++)
  {
    int const val = sudoku->grid[row][i];
    result = result | BITS[val];
  }
  return (~result) & 511;
}

int box_available(Sudoku const *const sudoku, const int row, const int col)
{
  const int left = (col / 3) * 3;
  const int top = (row / 3) * 3;
  int result = 0;
  for (int y = 0; y < 3; y++)
  {
    for (int x = 0; x < 3; x++)
    {
      int const val = sudoku->grid[top + y][left + x];
      result = result | BITS[val];
    }
  }
  return (~result) & 511;
}

/**
 * Solves a sudoku with the brute force backtracking (depth-fist search) algorithm recursively.
 *
 * @param sudoku Pointer to a sudoku struct to solve
 * @param row Starting row of solving algorithm
 * @param col Starting column of the solving algorithm
 */
bool backtrack(Sudoku *const sudoku, const int row, const int col)
{
  if (row < 0 || row > 8 || col < 0 || col > 8)
    return true;

  const int nextCol = col < 8 ? col + 1 : 0;
  const int nextRow = col >= 8 ? row + 1 : row;
  if (sudoku->locked[row][col])
    return backtrack(sudoku, nextRow, nextCol);

  if (!is_valid(sudoku))
    return false;

  const int avail = column_available(sudoku, col) & row_available(sudoku, row) & box_available(sudoku, row, col);
  for (int i = 1; i < 10; i++)
  {
    if ((BITS[i] & avail) != 0)
    {
      sudoku->grid[row][col] = i;
      const bool ret = backtrack(sudoku, nextRow, nextCol);
      if (ret)
        return true;
    }
  }
  sudoku->grid[row][col] = 0;
  return false;
}

bool valid_row(Sudoku const *const sudoku, const int row)
{
  int values = 0;
  for (int i = 0; i < 9; i++)
  {
    const int v = sudoku->grid[row][i];
    if ((BITS[v] & values) != 0)
      return false;
    values = values | BITS[v];
  }
  return true;
}

bool valid_column(Sudoku const *const sudoku, const int column)
{
  int values = 0;
  for (int i = 0; i < 9; i++)
  {
    const int v = sudoku->grid[i][column];
    if ((BITS[v] & values) != 0)
      return false;

    values = values | BITS[v];
  }
  return true;
}

bool valid_box(Sudoku const *const sudoku, const int row, const int column)
{
  const int left = (column / 3) * 3;
  const int top = (row / 3) * 3;
  int values = 0;
  for (int r = 0; r < 3; r++)
  {
    for (int c = 0; c < 3; c++)
    {
      const int v = sudoku->grid[top + r][left + c];
      if ((BITS[v] & values) != 0)
        return false;

      values = values | BITS[v];
    }
  }
  return true;
}

bool is_valid(Sudoku const *const sudoku)
{
  for (int row = 0; row < 9; row++)
  {
    if (!valid_row(sudoku, row))
      return false;
  }

  for (int column = 0; column < 9; column++)
  {
    if (!valid_column(sudoku, column))
      return false;
  }

  for (int box = 0; box < 9; box++)
  {
    if (!valid_box(sudoku, box, (box % 3) * 3))
      return false;
  }

  return true;
}
