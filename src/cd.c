#include <stdlib.h>
#include <string.h>
#include "file_system.h"
#include "commands.h"
#include <unistd.h>

extern "C" void fs_cd(struct file_system * F, FILE * fp, const char *dirname_const){
  //change current working directory to dirname
  int i;
  int flag = 1;
  struct directory dir;
  char * next;
  char * dirname;

  dirname = strdup(dirname_const);

  if(dirname[0]=='/') {
    F->cur_idx = F->root_idx;
    dirname = &dirname[1];
  }
  
  next = strchr(dirname,'/');
  if(next != NULL) {
    *next = '\0';
    next = &next[1];
  }

  //printf("Going to %s\n",dirname);
  fseek(fp, F->inode_list[F->cur_idx].direct[0], SEEK_SET);
  //printf("seek to %ld\n",ftell(fp));
  fread(&dir, 1, sizeof(struct directory), fp);
  rewind(fp);
  //printf("seek to %ld\n",ftell(fp));
  int r; 

  for(i=0;i<MAX_SIZE_DIRECTORY;i++) {
    //printf("%s\n",dir.files[i]);
    r=strcmp(dirname,dir.files[i]);
    //printf("%d\n",r);
    if(0==r/*strcmp(dirname,dir.files[i])*/) {
      /*if found set flag to zero and check to see if inode file type is directory or not.
        if inode file type is directory set file systems current directory to this directory's inode.*/
      flag = 0; 
      //printf("%s inode = %d\n",dir.files[i],dir.inodes[i]);
      if( F->inode_list[dir.inodes[i]].file_type == 1 ) {
        F->cur_idx = dir.inodes[i];
        if(next!=NULL) fs_cd(F,fp,next);
        //printf("going into location %d\n",F->cur_idx);
      } else {
        printf("Not a directory\n");
      }
    break;
    }
  }
  if(flag) printf("Directory not found\n");
}
