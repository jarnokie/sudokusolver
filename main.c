#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include "solver.h"
#include "exactcover.h"

enum Solver
{
  backtracking = 0,
  exactcover = 1
};

void print_help()
{
  printf("Usage: solver [-s solvername] sudokufile\n");
  printf("Available solvers: 'backtracking' (default), 'exactcover'\n");
}

/**
 * Fills a sudoku puzzle from the file given by the filename.
 *
 * @param filename Filename string
 * @param sudoku Pointer to the sudoku struct to be filled
 */
int read_sudoku_file(char const *const filename, Sudoku *const sudoku);

int main(int argc, char **argv)
{
  enum Solver solver = backtracking;

  // Parse args
  char *opts = "hs:";
  int c = getopt(argc, argv, opts);
  while (c != -1)
  {
    if (c == 's')
    {
      char const *solverArg = optarg;
      if (strcmp(solverArg, "backtracking") == 0)
      {
        solver = backtracking;
      }
      else if (strcmp(solverArg, "exactcover") == 0)
      {
        solver = exactcover;
      }
      else
      {
        printf("Invalid solver: %s\n", solverArg);
        return 1;
      }
    }
    else if (c == 'h')
    {
      print_help();
      return 1;
    }
    c = getopt(argc, argv, opts);
  }

  // Read the sudoku file
  Sudoku sudoku;
  char *filename = argv[optind];
  if (filename == NULL)
  {
    printf("No sudoku file given!\n");
    print_help();
    return 1;
  }
  int err = read_sudoku_file(argv[optind], &sudoku);
  if (err != 0)
    return err;

  print_sudoku(&sudoku);

  if (!is_valid(&sudoku))
  {
    printf("Invalid sudoku problem!\n");
    return 1;
  }

  printf("Solving...\n");

  bool success = false;

  // Use the selected solver
  if (solver == backtracking)
    success = backtrack(&sudoku, 0, 0);
  else if (solver == exactcover)
    success = exact_cover(&sudoku);

  if (!success)
  {
    printf("No solutions!\n");
    return 1;
  }

  // Print the solution
  print_sudoku(&sudoku);

  return 0;
}

int read_sudoku_file(char const *const filename, Sudoku *const sudoku)
{

  initialize_sudoku(sudoku);

  FILE *fp = fopen(filename, "r");

  if (fp == NULL)
  {
    printf("Error: could not open file %s", filename);
    return 1;
  }

  int c = fgetc(fp);
  int i = 0;
  while (c != EOF && i < 81)
  {
    switch (c)
    {
    case '0':
    case '.':
      // Empty grid spot;
      i++;
      break;

    default:
      // Check for numbers, otherwise skip
      if ('1' <= c && c <= '9')
      {
        int const row = i / 9;
        int const col = i % 9;
        sudoku->grid[row][col] = c - '0';
        sudoku->locked[row][col] = true;
        i++;
      }
    }

    c = fgetc(fp);
  }

  fclose(fp);

  return 0;
}
