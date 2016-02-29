/******************
  CS 560- PA #1
  mkdir.c
By: Joe Dorris
    Drew Masters
 *****************/

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
  struct directory dir2;
  //read in the current directory
  fseek(fp, F->inode_list[F->cur_idx].direct[0], SEEK_SET);
  fread(&dir, 1, sizeof(struct directory), fp);
  rewind(fp);
  
  for(i=0;i<MAX_SIZE_DIRECTORY;i++) {
    if(0==strcmp(dirname,dir.files[i])) {
      //file exists so nothing to be done 
      printf("File already exists\n");
    }   
  }   

  //search for open place in directory structure
  for(i=0;i<MAX_SIZE_DIRECTORY;i++) {
    if('\0'==dir.files[i][0]) {
      //when you find a open place find a inode not in use
	  //this inode is what will hold the directory
	  inode_num = get_inode(F);
      F->inode_list[inode_num].in_use=1;
      F->inode_list[inode_num].file_type=1;
      tmp=find_first_free_page(F);
      F->inode_list[inode_num].direct[0] = tmp;
	  F->inode_list[inode_num].size=sizeof(struct directory);
	  //write the filename and inode number to the parent directory's structure
      strcpy(dir.files[i],dirname);
      dir.inodes[i]=inode_num;
      break;
    }
  }

  //create a new directory structure and give it the inode that was found before
  dir2.inodes[0]=inode_num;
  dir2.inodes[1]=F->cur_idx;
  strcpy(dir2.files[0],".");
  strcpy(dir2.files[1],"..");
  for(j=2;j<MAX_SIZE_DIRECTORY;j++) {
    dir2.files[j][0]='\0';
  }

  //write the original directory back to disk
  fseek(fp, F->inode_list[F->cur_idx].direct[0], SEEK_SET);
  fwrite(&dir, sizeof(struct directory), 1, fp);
  rewind(fp);

  //write the new directory back to disk
  fseek(fp, F->inode_list[inode_num].direct[0], SEEK_SET);
  fwrite(&dir2, sizeof(struct directory), 1, fp);
  rewind(fp);
}
