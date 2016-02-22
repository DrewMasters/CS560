CC=g++
CFLAGS=-g
OBJ=obj
SRC=src
INC=include
LIB=lib
BIN=bin

_OBJS=file_system.o mkfs.o cd.o ls.o mkdir.o
OBJS = $(patsubst %,$(OBJ)/%,$(_OBJS))

LIBS=libfs.a

all: file_system

$(OBJ)/%.o: $(SRC)/%.c 
	$(CC) -c -I$(INC) -o $@ $< $(CFLAGS)

$(LIB): $(OBJS)
	ar rcs $(LIB)/libfs.a $(OBJS)

file_system: $(LIB)
	g++ -o $(BIN)/sh -I$(INC) $(OBJS) $(SRC)/sh.cpp
	#g++ -o $(BIN)/sh -I$(INC) obj/mkfs.o $(SRC)/sh.cpp

clean:
	rm $(OBJ)/* $(LIB)/* $(BIN)/* 
