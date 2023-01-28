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

START_TEST(test_knuths_algorithm_x)
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

  IntList *const selected = list_new();

  bool rows[] = {false, false, false, false, false, false};
  bool cols[] = {false, false, false, false, false, false, false};

  knuths_alg_x(mat, row_count, col_count, rows, cols, selected);

  ck_assert_int_eq(list_length(selected), 3);
  ck_assert_int_eq(list_contains(selected, 1), true);
  ck_assert_int_eq(list_contains(selected, 3), true);
  ck_assert_int_eq(list_contains(selected, 5), true);

  // TODO remove debug print:
  printf("selected:\n");
  IntListIter *iter = selected->first;
  while (iter != NULL)
  {
    int const row = iter->val;
    printf("%d: ", row);
    for (int col = 0; col < col_count; col++)
    {
      printf("%d ", mat[row][col]);
    }
    printf("\n");
    iter = iter->next;
  }
  // End of debug print.

  for (int row = 0; row < row_count; row++)
  {
    free(mat[row]);
  }
  free(mat);
  mat = NULL;
}
END_TEST

Suite *exact_cover_suite(void)
{
  Suite *s;
  TCase *tc_core;

  s = suite_create("Exact cover");

  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_knuths_algorithm_x);

  suite_add_tcase(s, tc_core);

  return s;
}
