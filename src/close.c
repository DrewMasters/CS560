/******************
 *   CS 560- PA #1
 *   close.c
 *   By: Joe Dorris
 *   Drew Masters
 *******************/

#include <stdlib.h>
#include "file_system.h"
#include "commands.h"
#include <unistd.h>

extern "C" void fs_close(FILE * fp, struct file_system * F, int fd){
  //if it is a valid file descriptor than close it
  if((fd>=0) && (fd<NUM_FILE_DESC)) free_fd(F,fd);
  return;
}
