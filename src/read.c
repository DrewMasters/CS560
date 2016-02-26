#include <stdlib.h>
#include "file_system.h"
#include "commands.h"
#include <unistd.h>

extern "C" char *fs_read(FILE * fp, struct file_system *F, int file_d, int size){
  //read the number of bytes specified by user with size argument
  //from the file descriptor.  return the read bytes. 
  //current file offset is incremented by size 
  
	//char rt[size];
	char *rt;
	rt = (char *)malloc(sizeof(char)*(size+1)); 
	char tmp;
  int idx, i,page_remaining;
	int csize=size;
	int direct_index=F->fd[file_d].in_offset/PAGE_SIZE;


	/*if (F->fd[file_d].type != 0){
		printf("Trying to read from write inode\n");
		return "\0";
	} else */
  if (F->fd[file_d].i->size==F->fd[file_d].in_offset){
		printf("Trying to read from file descriptor at end of file\n");
		return "\0";
	}

	fseek(fp,F->fd[file_d].out_offset,SEEK_SET);
  while (idx < size){
    page_remaining = PAGE_SIZE - (F->fd[file_d].in_offset % PAGE_SIZE);
    for (i=0; i<page_remaining; i++){
      if( ( (F->fd[file_d].in_offset) > F->fd[file_d].i->size) || (idx==size) ) {
        rt[idx]='\0';
        return rt;
      }
      fread(&(rt[idx]), 1, 1, fp);
      idx++;
      F->fd[file_d].i->size++;
      F->fd[file_d].in_offset++;
      F->fd[file_d].out_offset++;
    }   

    F->fd[file_d].out_offset = F->fd[file_d].i->direct[(F->fd[file_d].in_offset/PAGE_SIZE)];
    fseek(fp,F->fd[file_d].out_offset,SEEK_SET);
	}
  rt[idx]='\0';
	return rt;
}  
