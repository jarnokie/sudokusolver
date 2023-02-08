# Sudoku solver

This is a C programming learning project.
The goal is the implement different sudoku solvers with C. 

## Building

Run `make` and `make test` for unit test binary. Run test with `./test`.

Dependencies:
* subunit
* check library for tests.

## Usage

Sudoku puzzles are given as files. The file should contain each of the numbers in the puzzle from
left to right, top to bottom order with dots ('.') representing empty squares. See [test.sudoku](test.sudoku)
file for example. All other characters apart from numbers and dots are ignored.

To solve the sample problem run `./solver test.sudoku`. The default solver uses the depth-first
'backtracking' algorithm to solve the puzzle. See available solvers from the binary's help
`./solver -h`.
