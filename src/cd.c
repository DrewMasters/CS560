#include <stdlib.h>
#include <string.h>
#include "file_system.h"
#include "commands.h"
#include <unistd.h>

extern "C" void fs_cd(struct file_system * F, FILE * fp, const char *dirname){
  //change current working directory to dirname
  int i;
  int flag = 1;
  struct directory dir;
  //fseek(fp, F->inode_list[F->cur_idx].direct[0], SEEK_SET);
  //fread(&dir, 1, sizeof(struct directory), fp);
  fseek(fp, F->inode_list[0].direct[0], SEEK_SET);
  printf("seek to %ld\n",ftell(fp));
  printf("read in %lu\n",fread(&dir, 1, sizeof(struct directory), fp));
  rewind(fp);
  printf("seek to %ld\n",ftell(fp));
  
  for(i=0;i<MAX_SIZE_DIRECTORY;i++) {
    //printf("%s\n",dir.files[i]);
    if(0==strcmp(dirname,dir.files[i])) {
      /*if found set flag to zero and check to see if inode file type is directory or not.
        if inode file type is directory set file systems current directory to this directory's inode.*/
      flag = 0; 
      if( F->inode_list[dir.inodes[i]].file_type == 1 ) {
        F->cur_idx = dir.inodes[i];
      } else {
        printf("Not a directory\n");
      }
    }
  }
  if(flag) printf("Directory not found\n");
  rewind(fp);
}
