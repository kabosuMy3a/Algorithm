# Programming Assignment 1

We are group 2 in section 2 (mon, fri 6)  

21300362 Sangwon Seo  
21300858 Youngjae Kim  
21400667 Byoungun Jeon  
21500800 Sungyu Hong    

## Build & Excute

we used Makefile(CMake) as a build tool  

1. Enter "make all" in console where Makefile exists. Then Build tool compiles C code with gcc.
2. "Sudoku", "FillaPix", and "Numbrix" will be generated in now directory. They are compiled files.  
3. "Sudoku" is matched pa101. "FillaPix" is matched pa102. "Numbrix" is matced pa103.     

## Entering the Input 

All program receives the inputs by standard input. 

"Sudoku" receives numbers from 1 to 9. \*, and ? can be entered. Other charters are ignored.  
Sudoku's inputs are fixed in 81 characters, so Standard inputs are terminated when 9x9 grid is made.   

Otherwise, "FillaPix" and "Numbrix", their grid size is not fixed. Program doesn't know whether EOF.   
**So, please tester enter ```"Ctrl+D"``` after all inputs are entered.**  

"FillaPix" receives initial numbers from 0 to 9. ? can be entered for undefined value.  
"Numbrix" receives intial numbers from 1 to MAX, MAX means the number of cells. ? can be entered.  
When these two programs receive first row of grid, it will set up the column number.  
So, this program check whether next input's column number is same with first one.  
When they are different, program makes error message and receives input again.  

## Example

1. Build with Makefile.
```$ make all ```

2. Input your data that you want to solve
```
  $ ./FillaPix         
      ? ? ? ? 2   
      ? 9 ? ? ?  
      ? 8 8 ? ?  
      ? ? ? ? 4  
      4 ? 5 ? 2   
```

3. Press ```CTRL+D``` After you finished putting data.


4. You can get the result.

-    If puzzle don't have any solution, it will print "No solution"
-    If puzzle have a solution, it will be printed. Only "FIllaPix" prints 5 solutions.

## Files 

* [Makefile](Makefile) - Build script for CMake  
* [\[pa101.c\]](pa101/pa101.c) [\[pa102.c\]](pa102/pa102.c) [\[pa103.c\]](pa103/pa103.c) - source codes. They are in each of folders.  

##### After excute program. Some of below files are generated in each program folders.  
- Input shown as grid.  
- Logic and constraints represented by Z3 syntax for launching Z3  
- Raw output from Z3 launched inside C program. Model is given if satisfiable, else error is given.    
- Parsed info also printed in console. 
