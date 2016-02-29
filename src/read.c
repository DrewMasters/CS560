/******************
  CS 560- PA #1
  read.c
By: Joe Dorris
    Drew Masters
 *****************/

#include <stdlib.h>
#include "file_system.h"
#include "commands.h"
#include <unistd.h>

extern "C" char *fs_read(FILE * fp, struct file_system *F, int file_d, int size){
  //read the number of bytes specified by user with size argument
  //from the file descriptor.  return the read bytes. 
  //current file offset is incremented by size 

  char *rt;
  rt = (char *)malloc(sizeof(char)*(size+1)); 
  char tmp;
  int idx, i,page_remaining;
  int csize=size;
  int direct_index=F->fd[file_d].in_offset/PAGE_SIZE;

  idx=0;
  //if file descriptor is at the end of the file return
  if (F->fd[file_d].i->size==F->fd[file_d].in_offset){
		return "\0";
  }

  //go to location specified by file offset and while size read
  //in is smaller than the size of the file read in characters
  fseek(fp,F->fd[file_d].out_offset,SEEK_SET);
  while (idx < size){
    page_remaining = PAGE_SIZE - (F->fd[file_d].in_offset % PAGE_SIZE);
    for (i=0; i<page_remaining; i++){
	  if( ( (F->fd[file_d].in_offset) >= F->fd[file_d].i->size) || (idx==size) ) {
		//reached end of file return what has been read
        rt[idx]='\0';
        return rt;
      }
      fread(&(rt[idx]), 1, 1, fp);
      idx++;
      F->fd[file_d].in_offset++;
      F->fd[file_d].out_offset++;
    }   

    F->fd[file_d].out_offset = F->fd[file_d].i->direct[(F->fd[file_d].in_offset/PAGE_SIZE)];
    fseek(fp,F->fd[file_d].out_offset,SEEK_SET);
  }
  rt[idx]='\0';
  return rt;
}  
