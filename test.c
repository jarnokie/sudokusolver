#include <stdlib.h>
#include <check.h>
#include "test_sudoku.h"
#include "test_list.h"
#include "test_exact_cover.h"


int main(void) {
  int number_failed;
  Suite *solver, *list, *exact_cover;
  SRunner *sr;

  solver = solver_suite();
  list = list_suite();
  exact_cover = exact_cover_suite();

  sr = srunner_create(solver);
  srunner_add_suite(sr, list);
  srunner_add_suite(sr, exact_cover);

  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

