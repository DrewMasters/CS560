/******************
  CS 560- PA #1
  mkfs.c
  By: Joe Dorris
  Drew Masters
 *****************/

#include <stdlib.h>
#include "file_system.h"
#include "commands.h"
#include <unistd.h>
#include "string.h"

void fs_mkfs(FILE * fp, struct file_system * F) {
  //make or reformat a file
  //1. check to see if file exists
  //2. if file exists delete it
  //3. create file 
  //4. create root inode
  //5. write file_system structure 
  //6. create root directory
  //7. write root directory to root inode 
  
  struct directory dir;
  int j;
  long pos;
  
  //initialize inodes, file descriptors, and pages
  for(j=0;j<NUM_INODES;j++) {
    F->inode_list[j].in_use=0;
  }
  for(j=0;j<NUM_FILE_DESC;j++) {
    F->fd[j].in_use=0;
  }
  for(j=0;j<NUM_FREE_LIST_BYTES;j++) {
    F->free_list[j]=0;
  }

  //set up root inode
  F->root_idx = 0;
  F->cur_idx = 0;
  F->inode_list[0].in_use=1;
  F->inode_list[0].file_type=1;
  F->inode_list[0].direct[0] = find_first_free_page(F);
  F->inode_list[0].size=sizeof(struct directory);
  
  rewind(fp);
  //write filesystem to file
  fwrite(F, sizeof(struct file_system), 1, fp);
  //write memory
  for(j=0;j<DISK_SIZE;j++) {
    fputc('a',fp);
  }
  rewind(fp);

  //create root directory
  dir.inodes[0]=0;
  dir.inodes[1]=0;
  strcpy(dir.files[0],".");
  strcpy(dir.files[1],"..");
  for(j=2;j<MAX_SIZE_DIRECTORY;j++) {
    dir.files[j][0]='\0';
  }

  //write dir to F->inode_list[0]->direct[0]
  fseek(fp, F->inode_list[0].direct[0], SEEK_SET);
  fwrite(&dir, sizeof(struct directory), 1, fp);
  rewind(fp);

  return;
}
