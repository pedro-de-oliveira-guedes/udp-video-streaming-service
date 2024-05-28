CC = gcc
LIBS = -lm
CFLAGS = -pg -Wall -c -I $(INC)

SRC = ./src
OBJ = ./obj
INC = ./include
BIN = ./bin

MAIN_EXE = $(BIN)/main
MAIN_OBJ = $(OBJ)/main.o

all: folders $(MAIN_EXE)

folders:
	mkdir -p $(OBJ) $(BIN)

$(MAIN_EXE): $(MAIN_OBJ)
	$(CC) -pg -o $@ $^ $(LIBS)

$(MAIN_OBJ): $(SRC)/main.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf $(OBJ) $(BIN)