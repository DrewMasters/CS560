/******************
 *   CS 560- PA #1
 *   import.c
 *   By: Joe Dorris
 *   Drew Masters
 ******************/

#include <stdlib.h>
#include "file_system.h"
#include "commands.h"
#include <unistd.h>
#include <sys/stat.h>

extern "C" void fs_import(FILE * fp, struct file_system * F, const char *srcname, const char *destname) {
  FILE * import_file = fopen(srcname, "rb");
  if (import_file == NULL) {
    printf("Could not import\n");
    return;
  }
  char flag = 'w';
  struct stat info;
  
  //get the size of the file to import
  stat(srcname, &info);
  char *content = (char *)malloc(info.st_size * sizeof(char));

  //open a file in our file system
  int fd = fs_open(fp,F,destname,&flag);

  //read it into a buffer
  fread(content,info.st_size,1,import_file);
  //write it to the new file
  fs_write(fp,F,fd,content,info.st_size);
  
  //close file descriptors
  fs_close(fp,F,fd);
  fclose(import_file);
  return;
}
