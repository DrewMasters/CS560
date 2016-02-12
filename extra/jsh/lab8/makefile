#CS 360 Lab 8 Makefile

CC = gcc 

INCLUDES = -I/home/plank/cs360/include

CFLAGS = -g $(INCLUDES)

LIBDIR = /home/plank/cs360/objs

LIBS = $(LIBDIR)/libfdr.a

EXECUTABLES = jsh1 jsh2 jsh3


all: $(EXECUTABLES)

.SUFFIXES: .c .o
.c.o:
	$(CC) $(CFLAGS) -c $*.c

jsh1: jsh1.o
	$(CC) $(CFLAGS) -o jsh1 jsh1.o $(LIBS)

jsh2: jsh2.o
	$(CC) $(CFLAGS) -o jsh2 jsh2.o $(LIBS)

jsh3: jsh3.o
	$(CC) $(CFLAGS) -o jsh3 jsh3.o $(LIBS)

#make clean will rid your directory of the executable,
#object files, and any core dumps you've caused
clean:
	rm *.o $(EXECUTABLES)


