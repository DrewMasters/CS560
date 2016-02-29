/******************
  CS 560- PA #1
  open.c
By: Joe Dorris
    Drew Masters
 *****************/

#include <stdlib.h>
#include <string.h>
#include "file_system.h"
#include "commands.h"
#include <unistd.h>

extern "C" int fs_open(FILE * fp, struct file_system * F, const char *filename, const char *mode) {
  //create file_descriptor and return to caller
  int i;
  struct file_descriptor *fd;
  struct directory dir;
  int r;
  int flag;
  int fd_num;
  int inode_num;
  int tmp;

  flag = 0;
 
  //read in current directory
  fseek(fp, F->inode_list[F->cur_idx].direct[0], SEEK_SET);
  fread(&dir, 1, sizeof(struct directory), fp);
  rewind(fp);
 
  //find first free file descriptor
  fd_num=get_fd(F);
  if(fd_num==-1) return -1;

  //go through files in directory and find file called filename
  for(i=0;i<MAX_SIZE_DIRECTORY;i++) {
    r=strcmp(filename,dir.files[i]);
    if(0==r) {
      //if found check to see if it is a regular file
      if( F->inode_list[dir.inodes[i]].file_type==0) {
		//use file descriptor found above to store information about found file
        F->fd[fd_num].in_use=1;
        F->fd[fd_num].in_offset=0;
        F->fd[fd_num].i= &(F->inode_list[dir.inodes[i]]); 
        F->fd[fd_num].out_offset = F->inode_list[dir.inodes[i]].direct[0];
		//set what mode file descriptor is in
        if(mode[0]=='r') {
          F->fd[fd_num].type=0;
        }
        else if(mode[0]=='w') {
          F->fd[fd_num].type=1;
        }
      }
      else {
		//file is directory so free file descriptor and return
        free_fd(F,fd_num);
        return -1;
      }
    flag=1; 
    }
  }
  
  //if file not found than create a new file and use file descriptor from above
  if(flag==0) {
    if(mode[0]=='w') {
      F->fd[fd_num].in_use=1;
      F->fd[fd_num].type=1;
      F->fd[fd_num].in_offset=0;
      
      inode_num = get_inode(F);
      F->inode_list[inode_num].in_use=1;
      F->inode_list[inode_num].file_type=0;
      tmp=find_first_free_page(F);
      F->inode_list[inode_num].direct[0] = tmp;
      F->inode_list[inode_num].size=0; 

      for(i=0;i<MAX_SIZE_DIRECTORY;i++) {
        if('\0'==dir.files[i][0]) {
           strcpy(dir.files[i],filename);
           dir.inodes[i]=inode_num;
           break;
        }
      }      

      F->fd[fd_num].out_offset=F->inode_list[inode_num].direct[0];
      F->fd[fd_num].i = &F->inode_list[inode_num];
  
	  //write current inode and directory back to disk
      fseek(fp, F->inode_list[F->cur_idx].direct[0], SEEK_SET);
      fwrite(&dir, sizeof(struct directory), 1, fp);
      rewind(fp);
      
      //change current working directory to dirname
      int ix;
      struct directory dirx;
      fseek(fp, F->inode_list[F->cur_idx].direct[0], SEEK_SET);
      fread(&dirx, 1, sizeof(struct directory), fp);
      rewind(fp);
  
      for(ix=0;ix<MAX_SIZE_DIRECTORY;ix++) {
        if(dirx.files[ix][0] != '\0') {
          printf("%s %d\n",dir.files[ix],dirx.inodes[ix]);
        }   
      }
    }
    else {
      free_fd(F,fd_num);
      fd_num = -1;
    }
  }
  return fd_num;


  //use inode to get location of "file" in file hard drive and save the offset in offset in thefile_descriptor
  //save the inode that points to the file in the file_descriptor
  //save the file pointer to make for easier writing and reading
}
