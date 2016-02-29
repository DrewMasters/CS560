/******************
 *   CS 560- PA #1
 *   cd.c
 *   By: Joe Dorris
 *   Drew Masters
 ******************/

#include <stdlib.h>
#include <string.h>
#include "file_system.h"
#include "commands.h"
#include <unistd.h>

extern "C" void fs_cd(struct file_system * F, FILE * fp, const char *dirname_const){
  int i;
  int flag = 1;
  struct directory dir;
  char * next;
  char * dirname;

  dirname = strdup(dirname_const);

  //go to root if starts with slash
  if(dirname[0]=='/') {
    F->cur_idx = F->root_idx;
    dirname = &dirname[1];
  }
  
  //get path after the next slash for recursive call
  next = strchr(dirname,'/');
  if(next != NULL) {
    *next = '\0';
    next = &next[1];
  }

  fseek(fp, F->inode_list[F->cur_idx].direct[0], SEEK_SET);
  fread(&dir, 1, sizeof(struct directory), fp);
  rewind(fp);

  for(i=0;i<MAX_SIZE_DIRECTORY;i++) {
    if(0==strcmp(dirname,dir.files[i])) {
	  //found a file with this filenam
      flag = 0; 
	  //check if it is a directory
      if( F->inode_list[dir.inodes[i]].file_type == 1 ) {
		//set it as current directory
        F->cur_idx = dir.inodes[i];
		//continue cd if there is more path
        if(next!=NULL) fs_cd(F,fp,next);
      } else {
        printf("Not a directory\n");
      }
    break;
    }
  }
  //let user know that the path was not found
  if(flag) printf("Directory not found\n");
}
