#include <stdbool.h>
#include "solver.h"
#include "intlist.h"

bool exact_cover(Sudoku *const sudoku);

#ifdef UNIT_TEST
bool knuths_alg_x(bool **mat, int const h, int const w, bool rows[], bool cols[], IntList *selected);
#endif
