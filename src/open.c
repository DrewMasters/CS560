#include <stdlib.h>
#include "file_system.h"
#include "commands.h"
#include <unistd.h>

extern "C" struct file_descriptor fs_open(char *file_name) {
  //create file_descriptor and return to caller
  File *f;
  struct file_descriptor *fd;
  
  fd = (struct file_descriptor*)malloc(sizeof(struct file_descriptor)); 
  //use inode to get location of "file" in file hard drive and save the offset in offset in thefile_descriptor
  //save the inode that points to the file in the file_descriptor
  //save the file pointer to make for easier writing and reading
}
