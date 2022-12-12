#include <check.h>
#include "intlist.h"


START_TEST (test_add) {
    IntList * list = list_new(0);
    for (int i = 1; i < 10; i++) {
        list_add(list, i);
    }
    ck_assert_int_eq(list_length(list), 10);
    ck_assert_int_eq(list_get(list, 4), 4);
} END_TEST


START_TEST (test_del1) {
  IntList * list = list_new(0);
  for (int i = 1; i < 10; i++) {
      list_add(list, i);
  }
  ck_assert_ptr_eq(list_del(list, 5), list);
  int const len = list_length(list);
  ck_assert_int_eq(len, 9);
  for (int i = 0; i < len; i++) {
    if (i < 5) {
      ck_assert_int_eq(list_get(list, i), i);
    } else {
      ck_assert_int_eq(list_get(list, i), i+1);
    }
  }
} END_TEST


START_TEST (test_del2) {
  IntList * list = list_new(0);
  for (int i = 1; i < 10; i++) {
      list_add(list, i);
  }
  for (int i = 0; i < 9; i++) {
    ck_assert_ptr_eq(list_del(list, 0), list);
  }
  ck_assert_ptr_eq(list_del(list, 0), NULL);
} END_TEST


Suite * list_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("List");

  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_add);
  tcase_add_test(tc_core, test_del1);
  tcase_add_test(tc_core, test_del2);

  suite_add_tcase(s, tc_core);

  return s;
}
