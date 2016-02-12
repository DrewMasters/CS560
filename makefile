all: file_system

OBJS=file_system.o mkfs.o
LIB=libfs.a
INC=include

file_system.o:
	gcc -c -I$(INC) src/file_system.c

mkfs.o:
	gcc -c -I$(INC) src/mkfs.c

$(LIB): $(OBJS)
	ar rcs libfs.a $(OBJS)

file_system: $(LIB)
	gcc -o file_system -I./include -L./lib/file_system.a ./src/file_system.c
