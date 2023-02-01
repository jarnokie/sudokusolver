#ifndef SOLVER_H
#define SOLVER_H

#include <stdbool.h>

/**
 * Struct for Sudoku game state.
 */
typedef struct Sudoku
{
  int grid[9][9];
  bool locked[9][9];
} Sudoku;

/**
 * Initializes a Sudoko struct to empty state. All cells are set to 0 meaning no value assigned.
 *
 * @param sudoku Pointer to the Sudoku struct.
 */
void initialize_sudoku(Sudoku *const);

/**
 * Prints the Sudoku squares with the set values.
 *
 * @param sudoku Pointer to the Sudoku struct.
 */
void print_sudoku(Sudoku const *const);

/**
 * Solves the given sudoku with the brute force 'backtrack' algorithm.
 * A solution is guaranteed for valid problems.
 *
 * @param sudoku Pointer to a sudoku struct to be solved
 * @param row Starting row for the algorithm. Usually 0.
 * @param col Starting column for the algorithm. Usually 0.
 */
bool backtrack(Sudoku *const sudoku, int const row, int const col);

/**
 * Checks if a sudoku is valid.
 *
 * @param sudoku Pointer to a sudoku struct
 * @returns Is the given sudoku valid
 */
bool is_valid(Sudoku const *const sudoku);

#ifdef UNIT_TEST
bool valid_row(Sudoku const *const sudoku, const int row);
bool valid_column(Sudoku const *const sudoku, const int column);
bool valid_box(Sudoku const *const sudoku, const int row, const int column);

int column_available(Sudoku const *const sudoku, const int col);
int row_available(Sudoku const *const sudoku, const int row);
int box_available(Sudoku const *const sudoku, const int row, const int col);
#endif

#endif
