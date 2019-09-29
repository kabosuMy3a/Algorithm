# Programming Assignment 1

We are group 2 in section 2 (mon, fri 6)  
'~'  
'~'  
'~'  
21500800 HongSungyu  

## Build & Excute

we used Makefile(CMake) as a build tool  

1. Enter "make all" in console where Makefile exists.  
2. "Sudoku", "FillaPix", and "Numbrix" will be generated in now directory. They are compiled files.  
3. "Sudoku" is matched pa101. "FillaPix" is matched pa102. "Numbrix" is matced pa103.     
4. Details of each assginment are written below.  

## pa101, Sudoku  

pa101 receives the input by standard input. Recieve only 81 characters because this puzzle size is fixed.  
Other puzzles are receives input differently, but this puzzles receives in this way.  
This program receives numbers from 1 to 9 as character. \*, and ? can be entered.  

#### In directory pa101, there are 6 files.   
* [pa101.c](pa101/pa101.c) - source code  
* [SudokuInputExample](pa101/SudokuInputExample.txt) - Input example from Sudoku game for testing.  
##### below 4 files are generated from source code  
* [input2Grid](pa101/input2Grid.txt) - Save & Shows input as 9x9 grid.  
* [Z3udoku](pa101/Z3udoku) - Logic and constraints represented by Z3 syntax for launching Z3  
* [rawdoku](pa101/rawdoku) - Raw output from Z3 launched inside C program. Model is given if satisfiable, else error is given.    
* [parsedOutput](pa101/parsedOutput.txt) - Parsed info also printed in stdout from rawdoku. If input makes constraints unsatisfiable,  
Custom message "There is no solution for your SUDOKU." is printed with Z3 error message.  

## pa102, FillaPix  

pa102 receives the input by standard input like pa101. However it's somehow different in entering inputs.     
**Please tester enter "Ctrl+D" after all inputs are entered**, becuase this puzzle's grid is not fixed.  
This program receives numbers from 0 to 9 as characters. ? can be entered.  
When this program receives first row of grid, it will set up the column number.  
So, this program check whether next input's column number is same with first one.  
When they are different, program makes error message and receives input again.  
This policies of program are all for making perfect grid.  

## pa103, Numbrix  

