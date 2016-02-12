CC=gcc
CFLAGS=-g
OBJ=obj
SRC=src
INC=include
LIB=lib
BIN=bin

_OBJS=file_system.o
OBJS = $(patsubst %,$(OBJ)/%,$(_OBJS))

LIBS=libfs.a

all: file_system

$(OBJ)/%.o: $(SRC)/%.c 
	$(CC) -c -I$(INC) -o $@ $< $(CFLAGS)

$(LIB): $(OBJS)
	ar rcs $(LIB)/libfs.a $(OBJS)

file_system: $(LIB)
	g++ -o $(BIN)/sh -I$(INC) -L$(LIB) -lfs  $(SRC)/sh.cpp
