#include <stdlib.h>
#include <string.h>
#include "file_system.h"
#include "commands.h"
#include <unistd.h>

extern "C" void fs_ls(FILE * fp, struct file_system * F){

  //change current working directory to dirname
  int i;
  struct directory dir;
  //printf("printing for idx=%d\n",F->cur_idx);
  //fflush(stdout);
  fseek(fp, F->inode_list[F->cur_idx].direct[0], SEEK_SET);
  fread(&dir, 1, sizeof(struct directory), fp);
  rewind(fp);
  
  for(i=0;i<MAX_SIZE_DIRECTORY;i++) {
    if(dir.files[i][0] != '\0') {
      printf("%s\n",dir.files[i]);//,dir.inodes[i]);
    }
  }
}
