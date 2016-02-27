#include <stdlib.h>
#include "file_system.h"
#include "commands.h"
#include <unistd.h>

extern "C" void fs_export(FILE * fp, struct file_system * F, const char *srcname, const char *destname){
  //export a file (srcname) to the host machine (destname)
  char flag = 'r';
  FILE *export_file = fopen(destname, "w");
  if (export_file == NULL) {
    printf("Could not export\n");
    return;
  }
  int fd = fs_open(fp,F,srcname,&flag);
  fprintf(export_file, "%s",fs_read(fp,F,fd,F->fd[fd].i->size));
  fs_close(fp,F,fd);
  fclose(export_file);
  return;
  
}
