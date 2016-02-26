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
  
  fseek(fp, F->inode_list[F->cur_idx].direct[0], SEEK_SET);
  fread(&dir, 1, sizeof(struct directory), fp);
  rewind(fp);
 
  fd_num=get_fd(F);
  if(fd_num==-1) return -1;
  printf("got file descriptor %d\n",fd_num);

  for(i=0;i<MAX_SIZE_DIRECTORY;i++) {
    r=strcmp(filename,dir.files[i]);
    if(0==r) {
      if( F->inode_list[dir.inodes[i]].file_type==0) {
        F->fd[fd_num].in_use=1;
        F->fd[fd_num].in_offset=0;
        F->fd[fd_num].i= &(F->inode_list[dir.inodes[i]]); 
        F->fd[fd_num].out_offset = F->inode_list[dir.inodes[i]].direct[0];
        if(mode[0]=='r') {
          F->fd[fd_num].type=0;
        }
        else if(mode[0]=='w') {
          F->fd[fd_num].type=1;
        }
      }
      else {
        free_fd(F,fd_num);
        return -1;
      }
    flag=1; 
    }
  }

  if(flag==0) {
    if(mode[0]=='w') {
      F->fd[fd_num].in_use=1;
      F->fd[fd_num].type=1;
      F->fd[fd_num].in_offset=0;
      
      inode_num = get_inode(F);
      printf("trying inode %d\n",inode_num);
      F->inode_list[inode_num].in_use=1;
      F->inode_list[inode_num].file_type=0;
      tmp=find_first_free_page(F);
      printf("tmp: %d\n",tmp);
      F->inode_list[inode_num].direct[0] = tmp;//find_first_free_page(F);
      F->inode_list[inode_num].size=0; 

      for(i=0;i<MAX_SIZE_DIRECTORY;i++) {
        if('\0'==dir.files[i][0]) {
           strcpy(dir.files[i],filename);
           dir.inodes[i]=inode_num;
           printf("dir.inodes[%d]=%d\nfilename=%s\n",i,dir.inodes[i],filename);      
           break;
        }
      }      

      F->fd[fd_num].out_offset=F->inode_list[inode_num].direct[0];
      F->fd[fd_num].i = &F->inode_list[inode_num];
  
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
  
      //fs_ls(fp,F);
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
