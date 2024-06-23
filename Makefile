CC = gcc
LIBS = -lm
CFLAGS = -pg -Wall -c -I $(INC)

SRC = ./src
OBJ = ./obj
INC = ./include
BIN = ./bin

STREAMING_CLIENT_OBJS = $(OBJ)/movie.o $(OBJ)/catalog.o $(OBJ)/network_config.o $(OBJ)/client.o $(OBJ)/streaming_client.o
STREAMING_CLIENT_HDRS = $(INC)/movie.h $(INC)/catalog.h $(INC)/network_config.h $(INC)/client.h $(INC)/streaming_client.h
STREAMING_CLIENT_EXE = $(BIN)/client

STREAMING_SERVER_OBJS = $(OBJ)/movie.o $(OBJ)/catalog.o $(OBJ)/network_config.o $(OBJ)/server.o $(OBJ)/streaming_server.o
STREAMING_SERVER_HDRS = $(INC)/movie.h $(INC)/catalog.h $(INC)/network_config.h $(INC)/server.h $(INC)/streaming_server.h
STREAMING_SERVER_EXE = $(BIN)/server

all: folders $(STREAMING_CLIENT_EXE) $(STREAMING_SERVER_EXE)

folders:
	mkdir -p $(OBJ) $(BIN)

$(STREAMING_CLIENT_EXE): $(STREAMING_CLIENT_OBJS)
	$(CC) -pg -o $(STREAMING_CLIENT_EXE) $(STREAMING_CLIENT_OBJS) $(LIBS)

$(STREAMING_SERVER_EXE): $(STREAMING_SERVER_OBJS)
	$(CC) -pg -o $(STREAMING_SERVER_EXE) $(STREAMING_SERVER_OBJS) $(LIBS)

$(OBJ)/%.o: $(SRC)/%.c $(STREAMING_CLIENT_HDRS) $(STREAMING_SERVER_HDRS)
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJ) $(BIN)