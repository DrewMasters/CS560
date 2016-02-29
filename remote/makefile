#CS 360 Lab A Makefile

CC = gcc 

CFLAGS = -g

EXECUTABLES = server client

all: $(EXECUTABLES)

.SUFFIXES: .c .o
.c.o:
	$(CC) $(CFLAGS) -c $*.c

server: server.o sockettome.o
	$(CC) $(CFLAGS) -o server server.o sockettome.o

client: client.o sockettome.o
	$(CC) $(CFLAGS) -o client client.o sockettome.o

#make clean will rid your directory of the executable,
#object files, and any core dumps you've caused
clean:
	rm *.o $(EXECUTABLES)


