# Vector Calculator (minimat)

CPE 2600 Systems Programming - Mac Patterson

## Overview

minimat is an interactive command-line calculator for 3-dimensional vectors. At the minimat> prompt you can create named vectors, add, subtract, and scale them, and save or load them from CSV files. Each vector has a name and three double components.

Each line you type is split on spaces and commas, so put spaces around =, +, -, and *. A result that is not assigned to a name is displayed as ans.

## Building

Requires gcc and make.
- Build the program: make
- Remove object files and the executable: make clean

## Running

- Start the calculator: ./minimat
- Print help and exit: ./minimat -h
- Check for memory errors and leaks: valgrind --leak-check=full ./minimat

Any other command-line arguments print a usage message.

## Commands

- Create or replace a vector: a = 1 2 3 (commas also work: a = 1, 2, 3)
- Display a vector: a
- Add two vectors: a + b
- Subtract two vectors: a - b
- Multiply a vector by a scalar: a * 2.5 or 2.5 * a
- Store the result of an operation: c = a + b
- Display all stored vectors: list
- Remove all stored vectors: clear
- Load vectors from a CSV file: load file.csv
- Save all stored vectors to a CSV file: save file.csv
- Display help: help
- Exit the program: quit (Ctrl-D also exits)

### Vector Names

- 1 to 31 characters: letters, digits, or underscores
- Must start with a letter or underscore
- quit, clear, list, help, load, and save are reserved

### CSV File Format

Each line holds one vector written as name,x,y,z (for example: a,1,2,3).

- Loading adds to the vectors already in memory; a vector with the same name is replaced.
- Blank lines are ignored, and Windows (CRLF) line endings are accepted.
- Invalid lines are reported by line number and skipped.
- If the file cannot be opened, an error is printed and memory is unchanged.

## Dynamic Memory

1. Vectors are stored in an array on the heap (storage.c) instead of the fixed array of 10 used in Lab 5, so there is no limit on the number of vectors.
2. The array starts empty (NULL). Adding the first vector allocates room for 4, and each time the array fills up its capacity is doubled with realloc. Doubling keeps the number of reallocations small when loading large files.
3. The result of realloc is stored in a temporary pointer first, so if allocation fails the existing vectors are kept and the program reports that memory is full.
4. clear frees the array and resets the count and capacity to zero.
5. main registers clear with atexit, so the array is freed whenever the program exits normally. Valgrind reports "All heap blocks were freed -- no leaks are possible."