#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <check.h>

#include "exactcover.h"
#include "intlist.h"

/**
 * Helper funtion for getting the Wikipedia problem values.
 */
bool get_test_problem_value(int const index)
{
  bool test_problem[] = {
      true,
      false,
      false,
      true,
      false,
      false,
      true,
      true,
      false,
      false,
      true,
      false,
      false,
      false,
      false,
      false,
      false,
      true,
      true,
      false,
      true,
      false,
      false,
      true,
      false,
      true,
      true,
      false,
      false,
      true,
      true,
      false,
      false,
      true,
      true,
      false,
      true,
      false,
      false,
      false,
      false,
      true,
  };
  return test_problem[index];
}

START_TEST(test_knuths_algorithm_x1)
{
  // Try solving the Knuth's Agorith X Wikipedia page problem.
  //    1 2 3 4 5 6 7
  // A: 1 0 0 1 0 0 1
  // B: 1 0 0 1 0 0 0
  // C: 0 0 0 1 1 0 1
  // D: 0 0 1 0 1 1 0
  // E: 0 1 1 0 0 1 1
  // F: 0 1 0 0 0 0 1
  // Correct solution is B, D, F = 1, 3, 5
  int const row_count = 6, col_count = 7;
  bool **mat = (bool **)malloc(sizeof(bool *) * row_count);

  for (int row = 0; row < row_count; row++)
  {
    mat[row] = (bool *)malloc(sizeof(bool) * col_count);
    for (int col = 0; col < col_count; col++)
    {
      mat[row][col] = get_test_problem_value(row * col_count + col);
    }
  }

  IntList *selected = list_new();

  bool rows[] = {false, false, false, false, false, false};
  bool cols[] = {false, false, false, false, false, false, false};

  knuths_alg_x(mat, row_count, col_count, rows, cols, selected);

  ck_assert_int_eq(list_length(selected), 3);
  ck_assert_int_eq(list_contains(selected, 1), true);
  ck_assert_int_eq(list_contains(selected, 3), true);
  ck_assert_int_eq(list_contains(selected, 5), true);

  for (int row = 0; row < row_count; row++)
  {
    free(mat[row]);
  }
  free(mat);
  mat = NULL;

  list_free(selected);
  selected = NULL;
}
END_TEST

START_TEST(test_knuths_algorithm_x2)
{
  // Some problem but pre-select the row 4
  int const row_count = 6, col_count = 7;
  bool **mat = (bool **)malloc(sizeof(bool *) * row_count);

  for (int row = 0; row < row_count; row++)
  {
    mat[row] = (bool *)malloc(sizeof(bool) * col_count);
    for (int col = 0; col < col_count; col++)
    {
      mat[row][col] = get_test_problem_value(row * col_count + col);
    }
  }

  IntList *selected = list_new();

  bool rows[] = {false, false, false, false, false, false};
  bool cols[] = {false, false, false, false, false, false, false};
  remove_rows_cols(mat, &rows[0], &cols[0], row_count, col_count, 3);

  knuths_alg_x(mat, row_count, col_count, rows, cols, selected);

  ck_assert_int_eq(list_length(selected), 2);
  ck_assert_int_eq(list_contains(selected, 1), true);
  ck_assert_int_eq(list_contains(selected, 5), true);

  for (int row = 0; row < row_count; row++)
  {
    free(mat[row]);
  }
  free(mat);
  mat = NULL;

  list_free(selected);
  selected = NULL;
}
END_TEST

START_TEST(test_remove_rows_cols)
{
  int const row_count = 6, col_count = 7;
  bool **mat = (bool **)malloc(sizeof(bool *) * row_count);

  for (int row = 0; row < row_count; row++)
  {
    mat[row] = (bool *)malloc(sizeof(bool) * col_count);
    for (int col = 0; col < col_count; col++)
    {
      mat[row][col] = get_test_problem_value(row * col_count + col);
    }
  }
  bool rows[] = {false, false, false, false, false, false};
  bool cols[] = {false, false, false, false, false, false, false};
  remove_rows_cols(mat, &rows[0], &cols[0], row_count, col_count, 4);

  ck_assert_int_eq(rows[0], true);
  ck_assert_int_eq(rows[1], false);
  ck_assert_int_eq(rows[2], true);
  ck_assert_int_eq(rows[3], true);
  ck_assert_int_eq(rows[4], true);
  ck_assert_int_eq(rows[5], true);

  ck_assert_int_eq(cols[0], false);
  ck_assert_int_eq(cols[1], true);
  ck_assert_int_eq(cols[2], true);
  ck_assert_int_eq(cols[3], false);
  ck_assert_int_eq(cols[4], false);
  ck_assert_int_eq(cols[5], true);
  ck_assert_int_eq(cols[5], true);
}

START_TEST(test_exact_cover1)
{
  Sudoku sudoku;
  initialize_sudoku(&sudoku);

  exact_cover(&sudoku);

  ck_assert_int_eq(is_valid(&sudoku), true);
  for (int row = 0; row < 9; row++)
  {
    for (int col = 0; col < 0; col++)
    {
      ck_assert_int_ne(sudoku.grid[row][col], 0);
    }
  }
}

START_TEST(test_exact_cover2)
{
  Sudoku sudoku;
  initialize_sudoku(&sudoku);

  sudoku.grid[1][1] = 1;
  sudoku.locked[1][1] = true;

  exact_cover(&sudoku);

  ck_assert_int_eq(is_valid(&sudoku), true);
  ck_assert_int_eq(sudoku.grid[1][1], 1);
}

Suite *exact_cover_suite(void)
{
  Suite *s;
  TCase *tc_core;

  s = suite_create("Exact cover");

  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_knuths_algorithm_x1);
  tcase_add_test(tc_core, test_knuths_algorithm_x2);
  tcase_add_test(tc_core, test_remove_rows_cols);
  tcase_add_test(tc_core, test_exact_cover1);
  tcase_add_test(tc_core, test_exact_cover2);

  suite_add_tcase(s, tc_core);

  return s;
}
