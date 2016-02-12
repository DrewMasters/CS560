all: file_system

file_system:
	gcc -o file_system -I../include ./src/file_system.c
