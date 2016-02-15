#include <stdlib.h>
#include "file_system.h"
#include "commands.h"
#include <unistd.h>

extern "C" void fs_write(struct file_descriptor *fd, char *string){
  //wrtie string into file associated with file_descriptor
  //from current file offset.  current file offset moves forward
  //by the size of the string after write.  if end of file reached
  //size of file is increased
}
