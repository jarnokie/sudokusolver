#include <stdbool.h>
#include <check.h>

#include "exactcover.h"
#include "intlist.h"


START_TEST (test_knuths_algorithm_x) {
  bool mat[6][7] = {
    {true, false, false, true, false, false, true},
    {true, false, false, true, false, false, false},
    {false, false, false, true, true, false, true},

    {false, false, true, false, true, true, false},
    {false, true, true, false, false, true, true},
    {false, true, false, false, false, false, true},
  };
  //knuths_alg_x(mat, 6, 7, {false, false, false, false, false, false},
    //{false, false, false, false, false, false, false}, selected);
} END_TEST


Suite * exact_cover_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("Exact cover");

  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_knuths_algorithm_x);

  suite_add_tcase(s, tc_core);

  return s;
}
