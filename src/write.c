/******************
  CS 560- PA #1
  write.c
By: Joe Dorris
    Drew Masters
 *****************/

#include <stdlib.h>
#include <string.h>
#include "file_system.h"
#include "commands.h"
#include <unistd.h>

extern "C" void fs_write(FILE * fp, struct file_system * F, int file_d, const char * w_string, int size){
  //wrtie string into file associated with file_descriptor
  //from current file offset.  current file offset moves forward
  //by the size of the string after write.  if end of file reached
  //size of file is increased
  
  int i, total_written;
  int page_remaining; 
  total_written=0;

  if (F->fd[file_d].type !=1){
    printf("Trying to write to read inode\n");
    return;
  }
 
  if (F->fd[file_d].i->file_type==1) {
    printf("trying to write to a directory\n");
    return;
  }

  fseek(fp,F->fd[file_d].out_offset, SEEK_SET);
  while (total_written < size){
    page_remaining = PAGE_SIZE - (F->fd[file_d].in_offset % PAGE_SIZE);
    //the size left to be written is less than the page remaining
	//so all of it can be written to the same page
	//else write what you can to current page and allocate new page so that write can continue
	if( size <= page_remaining ) {
		fwrite(&(w_string[total_written]), size, 1, fp);
        total_written+=size;
		F->fd[file_d].i->size+=size;
		F->fd[file_d].in_offset+=size;
		F->fd[file_d].out_offset+=size;
	}
	else {
		fwrite(&(w_string[total_written]), page_remaining, 1, fp);
		total_written+=page_remaining;
		F->fd[file_d].i->size+=page_remaining;
		F->fd[file_d].in_offset+=page_remaining;
		F->fd[file_d].out_offset+=page_remaining;;
        
		//allocate new page
        if ((F->fd[file_d].in_offset/PAGE_SIZE) < NUM_DIRECT_LINKS){
			F->fd[file_d].i->direct[(F->fd[file_d].in_offset/PAGE_SIZE)]=find_first_free_page(F);
			F->fd[file_d].out_offset = F->fd[file_d].i->direct[(F->fd[file_d].in_offset/PAGE_SIZE)] + F->fd[file_d].in_offset % PAGE_SIZE;
			fseek(fp,F->fd[file_d].out_offset, SEEK_SET);
		}
		else {
			printf("Only able to write %d bytes\n", total_written);
			if(F->fd[file_d].in_offset > F->fd[file_d].i->size) F->fd[file_d].i->size = F->fd[file_d].in_offset;
			return;
		}
    }
  }
  if(F->fd[file_d].in_offset > F->fd[file_d].i->size) F->fd[file_d].i->size = F->fd[file_d].in_offset;
  return;
}
