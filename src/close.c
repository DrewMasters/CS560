#include <stdlib.h>
#include "file_system.h"
#include "commands.h"
#include <unistd.h>

extern "C" void fs_close(FILE * fp, struct file_system * F, int fd){
  //close file_descriptor
  free_fd(F,fd);
  return;
}
