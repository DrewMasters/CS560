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
  
  //struct inode *i;
  struct directory dir;
  int j;
 
  //inode in filesystem
  //i = (struct inode*)malloc(sizeof(struct inode));
  //strcpy(F->inode_list[0]->file_name,"root");

  free(F);
  F = (struct file_system*)malloc(sizeof(struct file_system));
  rewind(fp);

  //write filesystem to file
  fwrite(F, sizeof(struct file_system), 1, fp);

  //write root inode to file
  //fwrite(i, sizeof(struct inode), 1, fp); 
 
  //write memory
  for(j=0;j<DISK_SIZE;j++) {
    fputc(0,fp);
  }
  rewind(fp);

  //write directory to inode
  //idx = get_inode;
  F->root_idx = 0;
  F->cur_idx = 0;
  F->inode_list[0].self=0;
  F->inode_list[0].file_type=1;
  F->inode_list[0].direct[0] = find_first_free_page(F);
  F->inode_list[0].size=sizeof(struct directory);
  dir.inodes[0]=0;
  dir.inodes[1]=0;
  strcpy(dir.files[0],".");
  strcpy(dir.files[1],"..");

  //write dir to F->inode_list[0]->direct[0]
  fseek(fp, F->inode_list[0].direct[0], DISK_OFFSET);
  fwrite(&dir, sizeof(struct directory), 1, fp);
  rewind(fp);

  return;
}
