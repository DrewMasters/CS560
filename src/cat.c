/******************
 *   CS 560- PA #1
 *   ls.c
 *   By: Joe Dorris
 *   Drew Masters
 ******************/

#include <stdlib.h>
#include "file_system.h"
#include "commands.h"
#include <unistd.h>

extern "C" void fs_cat(FILE * fp, struct file_system * F, const char *filename){
	char flag = 'r';
	int fd = fs_open(fp,F,filename,&flag);
	//if received a valid file descriptor
	if(fd != -1) {
		//read and print all of the contents of it
		printf("%s",fs_read(fp,F,fd,F->fd[fd].i->size));
		fs_close(fp,F,fd);
	}
	return;
}
