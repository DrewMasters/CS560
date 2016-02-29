/******************
 *   CS 560- PA #1
 *   file_system.c
 *   By: Joe Dorris
 *   Drew Masters
 ******************/

#include "file_system.h"

//set the free page bit to 0
void set_page_free(struct file_system *fs, long num) {
  int div_eight=num/8;
  int mod_eight=num%8;
  fs->free_list[div_eight]&=~(1<<(7-mod_eight));
}

//free a specified page number
void free_page(struct file_system *fs, long num) {
  num = num - sizeof(struct file_system);
  num = num/PAGE_SIZE;
  set_page_free(fs,num);
}

//find first page that is 0
int find_first_free_page(struct file_system *fs)
{
//finds first free page in free list
  int pos = 0; //position in array with an open array
  int i;
  int j=0; //position of first zero in number

  for (i=0; i < NUM_FREE_LIST_BYTES; i++)
  {
    if (fs->free_list[i]!=255)
    {
      pos = i;
      break;
    }
  }

  while(fs->free_list[pos] & (1<<(7-j))) {
    j++;
  }

  set_page_used(fs, pos*8+j); //set the page as taken
  return (int)sizeof(struct file_system) + (PAGE_SIZE*(pos*8+j)); //first free page number 
}

//set the free page bit to 0
void set_page_used(struct file_system *fs, long num) {
// and with ~1
  int div_eight=num/8;
  int mod_eight=num%8;
  fs->free_list[div_eight]|=(1<<(7-mod_eight));
}

//search the inode list of an inode that is not in use
int get_inode(struct file_system * F) {
  int i;
  for(i=0;i<NUM_INODES;i++) {
    if(F->inode_list[i].in_use==0) {
	  //set it to used and return the index
      F->inode_list[i].in_use = 1;
      return i;
    }
  }
  printf("out of inodes\n");
}

//get file descriptor that is not in use
int get_fd(struct file_system * F) {
  int i;
  for(i=0;i<NUM_FILE_DESC;i++) {
    if(F->fd[i].in_use==0) {
      F->fd[i].in_use = 1;
      return i;
    }
  }
  printf("out of fds\n");
  return -1;
}

//set a file descriptor to not in use
void free_fd(struct file_system * F,int i) {
  F->fd[i].in_use = 0;
}

//when exiting, write the file system struct back to the disk file
extern "C" void fs_exit(FILE * fp, struct file_system * F) {
  fseek(fp, 0, SEEK_SET);
  fwrite(F, sizeof(struct file_system), 1, fp);
  rewind(fp);
}

