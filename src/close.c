#include <stdlib.h>
#include "file_system.h"
#include "commands.h"
#include <unistd.h>

extern "C" void fs_close(struct file_descriptor *fd){
  //close file_descriptor
  //just free file_descriptor?
  free(fd);
  return;
}
