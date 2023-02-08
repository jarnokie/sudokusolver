#ifndef EXACT_COVER_H
#define EXACT_COVER_H

#include <stdbool.h>
#include "solver.h"
#include "intlist.h"

bool exact_cover(Sudoku *const sudoku);

#ifdef UNIT_TEST
bool knuths_alg_x(bool **mat, int const height, int const width, bool rows[], bool cols[], IntList *const selected, int *iter);
void remove_rows_cols(bool **mat, bool *rows, bool *cols, int const height, int const width, int const row);
#endif

#endif