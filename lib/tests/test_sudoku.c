#include <check.h>
#include "../solver.h"

void set_test_sudoku(Sudoku *const sudoku)
{
  initialize_sudoku(sudoku);
  const int data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9,
                      4, 5, 6, 7, 8, 9, 1, 2, 3,
                      7, 8, 9, 1, 2, 3, 4, 5, 6,
                      2, 1, 4, 3, 6, 5, 8, 9, 7,
                      3, 6, 5, 8, 9, 7, 2, 1, 4,
                      8, 9, 7, 2, 1, 4, 3, 6, 5,
                      5, 3, 1, 6, 4, 2, 9, 7, 8,
                      6, 4, 2, 9, 7, 8, 5, 3, 1,
                      9, 7, 8, 5, 3, 1, 6, 4, 2};
  int i = 0;
  for (int row = 0; row < 9; row++)
  {
    for (int col = 0; col < 9; col++)
    {
      sudoku->grid[row][col] = data[i++];
    }
  }
}

START_TEST(test_initialize_sudoku)
{
  Sudoku sudoku;
  initialize_sudoku(&sudoku);
  for (int row = 0; row < 9; row++)
  {
    for (int col = 0; col < 9; col++)
    {
      ck_assert_int_eq(sudoku.grid[row][col], 0);
      ck_assert_int_eq(sudoku.locked[row][col], false);
    }
  }
}
END_TEST

START_TEST(test_available1)
{
  Sudoku sudoku;
  set_test_sudoku(&sudoku);
  const int col = 2;
  const int row = 4;
  const int expected = 5;
  sudoku.grid[row][col] = 0;
  ck_assert_int_eq(column_available(&sudoku, col), 1 << (expected - 1));
  ck_assert_int_eq(row_available(&sudoku, row), 1 << (expected - 1));
  ck_assert_int_eq(box_available(&sudoku, row, col), 1 << (expected - 1));
}
END_TEST

START_TEST(test_available2)
{
  Sudoku sudoku;
  set_test_sudoku(&sudoku);
  const int row = 0;
  for (int i = 0; i < 9; i++)
  {
    sudoku.grid[row][i] = 0;
  }
  for (int i = 0; i < 9; i++)
  {
    ck_assert_int_eq(column_available(&sudoku, i), 1 << (i));
  }
  ck_assert_int_eq(row_available(&sudoku, row), 511);
  ck_assert_int_eq(box_available(&sudoku, row, 1), 7);
  ck_assert_int_eq(box_available(&sudoku, row, 4), 56);
  ck_assert_int_eq(box_available(&sudoku, row, 7), 448);
}
END_TEST

START_TEST(test_available3)
{
  Sudoku sudoku;
  initialize_sudoku(&sudoku);
  ck_assert_int_eq(row_available(&sudoku, 4), 511);
  ck_assert_int_eq(column_available(&sudoku, 4), 511);
  ck_assert_int_eq(box_available(&sudoku, 4, 4), 511);
}
END_TEST

START_TEST(test_valid_row1)
{
  Sudoku sudoku;
  set_test_sudoku(&sudoku);
  for (int i = 0; i < 9; i++)
  {
    ck_assert_int_eq(valid_row(&sudoku, i), true);
  }
}
END_TEST

START_TEST(test_valid_row2)
{
  Sudoku sudoku;
  set_test_sudoku(&sudoku);

  sudoku.grid[0][3] = 5;
  sudoku.grid[2][4] = 1;
  sudoku.grid[4][5] = 3;
  sudoku.grid[6][6] = 1;
  sudoku.grid[8][7] = 3;

  for (int i = 0; i < 9; i++)
  {
    ck_assert_int_eq(valid_row(&sudoku, i), i % 2);
  }
}
END_TEST

START_TEST(test_valid_row3)
{
  Sudoku sudoku;
  initialize_sudoku(&sudoku);

  for (int i = 0; i < 9; i++)
  {
    ck_assert_int_eq(valid_row(&sudoku, i), true);
  }
}
END_TEST

START_TEST(test_valid_column1)
{
  Sudoku sudoku;
  set_test_sudoku(&sudoku);
  for (int i = 0; i < 9; i++)
  {
    ck_assert_int_eq(valid_column(&sudoku, i), true);
  }
}
END_TEST

START_TEST(test_valid_column2)
{
  Sudoku sudoku;
  set_test_sudoku(&sudoku);

  sudoku.grid[8][0] = 5;
  sudoku.grid[7][2] = 1;
  sudoku.grid[6][4] = 3;
  sudoku.grid[5][6] = 1;
  sudoku.grid[4][8] = 3;

  for (int i = 0; i < 9; i++)
  {
    ck_assert_int_eq(valid_column(&sudoku, i), i % 2);
  }
}
END_TEST

START_TEST(test_valid_column3)
{
  Sudoku sudoku;
  initialize_sudoku(&sudoku);

  for (int i = 0; i < 9; i++)
  {
    ck_assert_int_eq(valid_column(&sudoku, i), true);
  }
}
END_TEST

START_TEST(test_valid_box1)
{
  Sudoku sudoku;
  set_test_sudoku(&sudoku);
  for (int r = 0; r < 9; r++)
  {
    for (int c = 0; c < 9; c++)
    {
      ck_assert_int_eq(valid_box(&sudoku, r, c), true);
    }
  }
}
END_TEST

START_TEST(test_valid_box2)
{
  Sudoku sudoku;
  set_test_sudoku(&sudoku);

  sudoku.grid[0][0] = 5;
  sudoku.grid[2][6] = 5;
  sudoku.grid[4][4] = 5;
  sudoku.grid[6][2] = 5;
  sudoku.grid[8][8] = 5;

  for (int i = 0; i < 81; i++)
  {
    const int s = (i / 27) * 3 + (i % 9) / 3;
    ck_assert_int_eq(valid_box(&sudoku, i / 9, i % 9), s % 2);
  }
}
END_TEST

START_TEST(test_valid_box3)
{
  Sudoku sudoku;
  initialize_sudoku(&sudoku);

  for (int i = 0; i < 9 * 9; i++)
  {
    ck_assert_int_eq(valid_box(&sudoku, i / 9, i % 9), true);
  }
}
END_TEST

START_TEST(test_valid1)
{
  Sudoku sudoku;
  set_test_sudoku(&sudoku);
  ck_assert_int_eq(is_valid(&sudoku), true);
}
END_TEST

START_TEST(test_valid2)
{
  Sudoku sudoku;
  set_test_sudoku(&sudoku);
  sudoku.grid[0][1] = 1;
  ck_assert_int_eq(is_valid(&sudoku), false);
}
END_TEST

START_TEST(test_valid3)
{
  Sudoku sudoku;
  initialize_sudoku(&sudoku);
  ck_assert_int_eq(is_valid(&sudoku), true);
}
END_TEST

START_TEST(test_backtrack1)
{
  Sudoku sudoku;
  initialize_sudoku(&sudoku);
  ck_assert_int_eq(backtrack(&sudoku, 0, 0), true);
  ck_assert_int_eq(is_valid(&sudoku), true);
}
END_TEST

START_TEST(test_backtrack2)
{
  Sudoku sudoku;
  set_test_sudoku(&sudoku);
  for (int i = 0; i < 81; i++)
  {
    const int row = i / 9;
    const int col = i % 9;
    if (i % 2 == 0)
    {
      sudoku.grid[row][col] = 0;
    }
    else
    {
      sudoku.locked[row][col] = true;
    }
  }
  ck_assert_int_eq(backtrack(&sudoku, 0, 0), true);
  ck_assert_int_eq(is_valid(&sudoku), true);
}
END_TEST

START_TEST(test_backtrack3)
{
  Sudoku sudoku;
  initialize_sudoku(&sudoku);
  sudoku.grid[0][0] = 2;
  sudoku.grid[0][1] = 2;
  sudoku.locked[0][0] = true;
  sudoku.locked[0][1] = true;
  ck_assert_int_eq(backtrack(&sudoku, 0, 0), false);
  ck_assert_int_eq(is_valid(&sudoku), false);
}
END_TEST

Suite *solver_suite(void)
{
  Suite *s;
  TCase *tc_core;

  s = suite_create("Solver");

  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_initialize_sudoku);

  tcase_add_test(tc_core, test_available1);
  tcase_add_test(tc_core, test_available2);
  tcase_add_test(tc_core, test_available3);

  tcase_add_test(tc_core, test_valid_row1);
  tcase_add_test(tc_core, test_valid_row2);
  tcase_add_test(tc_core, test_valid_row3);

  tcase_add_test(tc_core, test_valid_column1);
  tcase_add_test(tc_core, test_valid_column2);
  tcase_add_test(tc_core, test_valid_column3);

  tcase_add_test(tc_core, test_valid_box1);
  tcase_add_test(tc_core, test_valid_box2);
  tcase_add_test(tc_core, test_valid_box3);

  tcase_add_test(tc_core, test_valid1);
  tcase_add_test(tc_core, test_valid2);
  tcase_add_test(tc_core, test_valid3);

  tcase_add_test(tc_core, test_backtrack1);
  tcase_add_test(tc_core, test_backtrack2);
  tcase_add_test(tc_core, test_backtrack3);

  suite_add_tcase(s, tc_core);

  return s;
}
