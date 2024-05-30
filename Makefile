CC = gcc
LIBS = -lm
CFLAGS = -pg -Wall -c -I $(INC)

SRC = ./src
OBJ = ./obj
INC = ./include
BIN = ./bin

MAIN_EXE = $(BIN)/main
MAIN_HDRS = $(INC)/movie.h
MAIN_OBJ = $(OBJ)/main.o $(OBJ)/movie.o

all: folders $(MAIN_EXE)

folders:
	mkdir -p $(OBJ) $(BIN)

$(MAIN_EXE): $(MAIN_OBJ)
	$(CC) -pg -o $(MAIN_EXE) $(MAIN_OBJ) $(LIBS)

$(MAIN_OBJ): $(SRC)/main.c
	$(CC) $(CFLAGS) $(SRC)/main.c -o $(OBJ)/main.o

$(OBJ)/movie.o: $(SRC)/movie.c $(MAIN_HDRS)
	$(CC) $(CFLAGS) $(SRC)/movie.c -o $(OBJ)/movie.o

clean:
	rm -rf $(OBJ) $(BIN)