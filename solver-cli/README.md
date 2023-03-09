# CLI tool

Simple command line tool for solving sudokus.

## Building

Run `make` to build the `solver` binary.

## Usage

Sudoku puzzles are given as files. The file should contain each of the numbers in the puzzle from
left to right, top to bottom order with dots ('.') representing empty squares. See [test.sudoku](test.sudoku)
file for example. All other characters apart from numbers and dots are ignored.

To solve the sample problem run `./solver test.sudoku`. The default solver uses the depth-first
'backtracking' algorithm to solve the puzzle. See available solvers from the binary's help
`./solver -h`.

