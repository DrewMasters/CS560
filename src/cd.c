#include <stdlib.h>
#include "file_system.h"
#include "commands.h"
#include <unistd.h>

extern "C" void fs_cd(struct file_system * F, FILE * fp, char *dirname){
  //change current working directory to dirname
  int i;
  int flag = 1;
  struct directory dir;
  fseek(fp, F->inode_list[F->cur_idx].direct[0], DISK_OFFSET);
  fread(&dir, sizeof(struct directory), 1, fp);
  
  for(i=0;i<MAX_SIZE_DIRECTORY;i++) {
    if(dirname == dir.files[i]) {
      flag = 0;
      if( F->inode_list[dir.inodes[i]].file_type == 1 ) {
        F->cur_idx = dir.inodes[i];
      } else {
        printf("Not a directory\n");
      }
    }
  }
  if(flag) printf("Directory not found\n");
}
