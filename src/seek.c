#include <stdlib.h>
#include "file_system.h"
#include "commands.h"
#include <unistd.h>

extern "C" void fs_seek(struct file_descriptor *fd, int offset){
  //move current file offset in fd to new file offset specified by offset
  //offset means the number of bytes from beginning of file
} 
