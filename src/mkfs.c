#include <stdlib.h>
#include "file_system.h"
#include "commands.h"
#include <unistd.h>
#include "string.h"

void fs_mkfs(FILE * fp, struct file_system * F) {
  //make or reformat a file
  //1. check to see if file exists
  //2. if file exists delete it
  //3. create file 
  //4. create root inode in file
  //5. create filesystem structure in file
  
  struct inode *i;
  
  //create root inode
  i = (struct inode*)malloc(sizeof(struct inode));
  i->self=2;
  i->file_type=1;
  strcpy(i->file_name,"root");

  free(F);
  F = (struct file_system*)malloc(sizeof(struct file_system));
  rewind(fp);

  //write filesystem to file
  fwrite(F, sizeof(struct file_system), 1, fp);

  //write root inode to file
  fwrite(i, sizeof(struct inode), 1, fp); 
 
  //write memory
  //fwrite(

  rewind(fp);

  free(i);
  return;
}
