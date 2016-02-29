/******************
 *   CS 560- PA #1
 *   Basic shell
 *   By: Joe Dorris
 *   Drew Masters
 ******************/

#include <stdlib.h>
#include <string.h>
#include "file_system.h"
#include "commands.h"
#include <unistd.h>

extern "C" void fs_ls(FILE * fp, struct file_system * F){
  int i;
  struct directory dir;
  
  //read in the current directory
  fseek(fp, F->inode_list[F->cur_idx].direct[0], SEEK_SET);
  fread(&dir, 1, sizeof(struct directory), fp);
  rewind(fp);
  
  //print everything that is not named '\0'
  for(i=0;i<MAX_SIZE_DIRECTORY;i++) {
    if(dir.files[i][0] != '\0') {
      printf("%s\n",dir.files[i]);//,dir.inodes[i]);
    }
  }
}
