#include <stdlib.h>
#include <check.h>
#include "test_sudoku.h"
#include "test_list.h"


int main(void) {
  int number_failed;
  Suite *solver;
  Suite *list;
  SRunner *sr;

  solver = solver_suite();
  list = list_suite();
  sr = srunner_create(solver);
  srunner_add_suite(sr, list);

  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

