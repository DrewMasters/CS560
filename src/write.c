#include <stdlib.h>
#include <string.h>
#include "file_system.h"
#include "commands.h"
#include <unistd.h>

extern "C" void fs_write(FILE * fp, struct file_system * F, int file_d, char * w_string){
  //wrtie string into file associated with file_descriptor
  //from current file offset.  current file offset moves forward
  //by the size of the string after write.  if end of file reached
  //size of file is increased
  
  int size, chunk_size, total_written;
  char *tmp;
  
  size=strlen(w_string);
  total_written=0;

  if (F->fd[file_d].type !=1){
    printf("Trying to write to read inode\n");
    return;
  }

  fseek(fp,F->fd[file_d].out_offset, SEEK_SET);
  while (size > 0){
    if(F->fd[file_d].i->size > F->fd[file_d].i->size - F->fd[file_d].in_offset % PAGE_SIZE/*write can occur within given page*/){
      fwrite(w_string, 1, sizeof(w_string), fp);
      F->fd[file_d].i->size=F->fd[file_d].i->size+size;
      F->fd[file_d].in_offset=F->fd[file_d].in_offset+size;
      F->fd[file_d].out_offset=F->fd[file_d].out_offset+size;
      return;
    }
    else if (/*write will span multiple pages*/){
      //write to fill up page
      tmp = (char *) malloc(F->fd[file_d].i->size - F->fd[file_d].in_offset % PAGE_SIZE);
      chunk_size = sizeof(tmp);
      total_written=total_written+chunk_size;
      fwrite(tmp, 1, sizeof(tmp), fp);
      free(tmp);
      //allocate new page
      if (!((F->fd[file_d].in_offset/PAGE_SIZE) + 1 > NUM_FREE_LIST_BYTES)){
        F->fd[file_d].i->direct[(F->fd[file_d].in_offset/PAGE_SIZE)+1]=find_first_free_page(F);
      }
      else {
        printf("Only able to write %d bytes\n", total_written);
	return;
      }
      F->fd[file_d].i->size=F->fd[file_d].i->size+size;
      F->fd[file_d].in_offset=F->fd[file_d].in_offset+size;
      F->fd[file_d].out_offset=F->fd[file_d].out_offset+size;
    }
    size = size - chunk_size;
  }
}
