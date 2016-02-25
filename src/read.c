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
	rt = (char *)malloc(sizeof(char)*size); 
	char tmp;
	int csize=size;
	int direct_index=F->fd[file_d].in_offset/PAGE_SIZE;


	if (F->fd[file_d].type != 0){
		printf("Trying to read from write inode\n");
		return "\0";
	} else if (F->fd[file_d].i->size==F->fd[file_d].in_offset){
		printf("Trying to read from file descriptor at end of file\n");
		return "\0";
	}

	fseek(fp,F->fd[file_d].out_offset,SEEK_SET);
  while (size > 0){
		fread(&tmp,1,sizeof(char), fp);
		rt[csize-size]=tmp;

		//check to see if you have reached the end of a page
		//than move to next page in direct array
		if (direct_index!=F->fd[file_d].in_offset/PAGE_SIZE){
			direct_index=F->fd[file_d].in_offset/PAGE_SIZE;
			if (F->fd[file_d].i->size > F->fd[file_d].in_offset){
				//if inode size is larger than file descriptor internal 
				//offset than haven't reached the end of the file
				F->fd[file_d].out_offset=F->fd[file_d].i->direct[direct_index];
				fseek(fp,F->fd[file_d].out_offset,SEEK_SET);
			}
			else {
				//just return what you were able to read before end of file
				printf("read past end of file\n");
				rt[csize-size-1]='\0';
				break;
			}
		}

		size=size-1;
		F->fd[file_d].in_offset=F->fd[file_d].in_offset+1;
	}

	return rt;
}  
