#include <stdlib.h>
#include "file_system.h"
#include "commands.h"
#include <unistd.h>

extern "C" void fs_cat(FILE * fp, struct file_system * F, const char *filename){
  //show the content of the file
  char flag = 'r';
  int fd = fs_open(fp,F,filename,&flag);
  printf("%s",fs_read(fp,F,fd,F->fd[fd].i->size));
  fs_close(fp,F,fd);
  return;
}
