#include <stdlib.h>
#include "file_system.h"
#include "commands.h"
#include <unistd.h>

extern "C" void fs_write(FILE * fp, struct file_system * F, int file_d, char * w_string){
  //wrtie string into file associated with file_descriptor
  //from current file offset.  current file offset moves forward
  //by the size of the string after write.  if end of file reached
  //size of file is increased
  

}
