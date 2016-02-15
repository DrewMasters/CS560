#include <stdlib.h>
#include "file_system.h"
#include "commands.h"
#include <unistd.h>

extern "C" void fs_rmdir(char *dirname){
  //remove sub-directory called dirname if it is empty
  //free inode and directory
}
