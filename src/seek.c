/******************
  CS 560- PA #1
  seek.c
By: Joe Dorris
    Drew Masters
 *****************/

#include <stdlib.h>
#include <string.h>
#include "file_system.h"
#include "commands.h"
#include <unistd.h>

extern "C" void fs_seek(FILE * fp, struct file_system * F, int file_d, int offset){
  //move current file offset in fd to new file offset specified by offset
  //offset means the number of bytes from beginning of file

  //check to see if offset is larger than current file size
  if (offset > F->fd[file_d].i->size){
    printf("Offset is past end of file\n");
    return;
  }

  //internal offset will equal the user specified offset
  F->fd[file_d].in_offset=offset;
  //external offset will equal the offset mod page size index in the direct array
  //this gives the current page that is being worked on
  F->fd[file_d].out_offset=F->fd[file_d].i->direct[offset/PAGE_SIZE]+(offset%PAGE_SIZE);
} 
