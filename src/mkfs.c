#include <stdlib.h>
#include "file_system.h"
#include "commands.h"
#include <unistd.h>

extern "C" void fs_mkfs(char *file_name) {
  //make or reformat a file
  //1. check to see if file exists
  //2. if file exists delete it
  //3. create file 
  //4. create root inode in file
  //5. create filesystem structure in file
  
  File *f;
  struct file_system *fs;
  struct inode *i;

  //if file does exist
  if ( access(file_name, F_OK) == 0 && unlink(file_name) !=0 ){
    //file exists but error occured while deleting 
  }
 
  //create file
  f = fopen(file_name, "w");

  //create filesystem
  fs = (struct file_system*)malloc(sizeof(struct file_system));


  //write filesystem to file
  fwrite(fs, sizeof(struct file_system), 1, f);

  //create root inode
  i = (struct inode*)malloc(sizeof(struct i));
  i->self=2;
  i->file_type=1;
  i->file_name="root";

  //write root inode to file
  fwrite(i, sizeof(struct inode), 1, f); 

  return;
  //printf("hello world\n");
}
