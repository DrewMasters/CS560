#include <stdlib.h>
#include "file_system.h"
#include "commands.h"
#include <unistd.h>

extern "C" void fs_import(FILE * fp, struct file_system * F, const char *srcname, const char *destname) {
  //import file (srcname) from host machine to current directory 
  //destname is what the file name will be in our file system
  FILE * import_file = fopen(srcname, "r");
  char flag = 'w';
  char c[2];
  c[1] = '\0';
  if (import_file == NULL) {
    printf("Could not import\n");
    return;
  }

  int fd = fs_open(fp,F,destname,&flag);
  while((c[0] = getc(import_file)) != EOF) {
    fs_write(fp,F,fd,c);
    //printf("%s\n",c);
  }
  fs_close(fp,F,fd);
  fclose(import_file);
  return;
  
  
}
