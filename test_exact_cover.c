#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <check.h>

#include "exactcover.h"
#include "intlist.h"

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

  for (int row = 0; row < row_count; row++)
  {
    free(mat[row]);
  }
  free(mat);
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
