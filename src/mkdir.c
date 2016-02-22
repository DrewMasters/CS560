#include <stdlib.h>
#include <string.h>
#include "file_system.h"
#include "commands.h"
#include <unistd.h>

extern "C" void fs_mkdir(FILE * fp, struct file_system * F, const char *dirname){
  //create sub-directory under current directory called dirname
  //create inode and directory structure
  int i,j;
  int inode_num;
  int flag = 1;
  int tmp;
  struct directory dir;
  fseek(fp, F->inode_list[F->cur_idx].direct[0], SEEK_SET);
  fread(&dir, 1, sizeof(struct directory), fp);
  rewind(fp);
  
  for(i=0;i<MAX_SIZE_DIRECTORY;i++) {
    if(0==strcmp(dirname,dir.files[i])) {
      //file exists so 
      printf("File already exists\n");
    }   
  }   
  for(i=0;i<MAX_SIZE_DIRECTORY;i++) {
    if('\0'==dir.files[i][0]) {
      printf("empty spot at %d\n",i);
      inode_num = get_inode(F);
      printf("trying inode %d\n",inode_num);
      F->inode_list[inode_num].in_use=1;
      F->inode_list[inode_num].file_type=1;
      tmp=find_first_free_page(F);
      printf("tmp: %d\n",tmp);
      F->inode_list[inode_num].direct[0] = tmp;//find_first_free_page(F);
      F->inode_list[inode_num].size=sizeof(struct directory); 
      strcpy(dir.files[i],dirname);
      dir.inodes[i]=inode_num;
      printf("dir.inodes[%d]=%d\n",i,dir.inodes[i]);
      break;
    }
  }

  struct directory dir2;

  dir2.inodes[0]=inode_num;
  dir2.inodes[1]=F->cur_idx;
  strcpy(dir2.files[0],".");
  strcpy(dir2.files[1],"..");

  for(j=2;j<MAX_SIZE_DIRECTORY;j++) {
    dir2.files[j][0]='\0';
  }

  printf("seek to %lu\n",F->inode_list[F->cur_idx].direct[0]);
  fseek(fp, F->inode_list[F->cur_idx].direct[0], SEEK_SET);
  fwrite(&dir, sizeof(struct directory), 1, fp);
  rewind(fp);
  //fseek(fp, F->inode_list[F->cur_idx].direct[0], SEEK_SET);
  //fread(&dir, 1, sizeof(struct directory), fp);
  
  printf("seek to %lu\n",F->inode_list[inode_num].direct[0]);
  fseek(fp, F->inode_list[inode_num].direct[0], SEEK_SET);
  fwrite(&dir2, sizeof(struct directory), 1, fp);
  rewind(fp);

  F->cur_idx = inode_num;
}
