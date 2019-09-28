CC = gcc
CFLAGS =
TARGET1 = Sudoku
TARGET2 = FillaFix
TARGET3 = Numbrix
OBJ1 = ./pa101/pa101.c
OBJ2 = ./pa102/pa102.c
OBJ3 = ./pa103/pa103.c

all : $(TARGET1) $(TARGET2) $(TARGET3)

$(TARGET1) : $(OBJ1)
	$(CC) $(CFLAGS) $(OPTION) -o $@ $^

$(TARGET2) : $(OBJ2)
	$(CC) $(CFLAGS) $(OPTION) -o $@ $^

$(TARGET3) : $(OBJ3)
	$(CC) $(CFLAGS) $(OPTION) -o $@ $^

clean : 
	rm Sudoku FillaFix Numbrix
