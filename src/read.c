#include <stdlib.h>
#include "file_system.h"
#include "commands.h"
#include <unistd.h>

extern "C" char *fs_open(struct file_descriptor *fd, int size){
  //read the number of bytes specified by user with size argument
  //from the file descriptor.  return the read bytes. 
  //current file offset is incremented by size 
}  
