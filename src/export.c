/******************
 *   CS 560- PA #1
 *   export.c
 *   By: Joe Dorris
 *   Drew Masters
 ******************/

#include <stdlib.h>
#include "file_system.h"
#include "commands.h"
#include <unistd.h>

extern "C" void fs_export(FILE * fp, struct file_system * F, const char *srcname, const char *destname){
  //export a file (srcname) to the host machine (destname)
  char flag = 'r';

  //open a new file on the host file system
  FILE *export_file = fopen(destname, "w");
  if (export_file == NULL) {
    printf("Could not export\n");
    return;
  }
  //open the src file for reading
  int fd = fs_open(fp,F,srcname,&flag);
  //read in the full file and write it to the new file
  fprintf(export_file, "%s",fs_read(fp,F,fd,F->fd[fd].i->size));
  //close file descriptors
  fs_close(fp,F,fd);
  fclose(export_file);
  return;
  
}
