#include <stdlib.h>
#include "file_system.h"
#include "commands.h"
#include <unistd.h>

extern "C" struct inode fs_mkdir(char *dirname){
  //create sub-directory under current directory called dirname
  //create inode and directory structure
