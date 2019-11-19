# HomeWork 2

\#Sec2
21500800 Sungyu Hong    

## Build & Excute

I used Makefile(CMake) as a build tool  

1. Enter "make" in console where Makefile exists. Then Build tool compiles C code with gcc.
2. "DNFConverter" will be generated in now directory. They are compiled files.  

## Entering the Input 

All program receives the inputs by standard input.  

## Example

1. Build with Makefile.
```$ make all ```

2. Input your data that you want to solve
```
  $ ./DNFConverter                       or   $./DNFConverter < input.txt
  (or (not (or (not (or a2 a3)) a4)))   
```

3. You can get the result.


## Files 

* [Makefile](Makefile) - Build script for CMake  
* [\[DNFConverter.c\]](DNFConverter.c) - source codes.

