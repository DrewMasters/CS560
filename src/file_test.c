#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "file_system.h"

int main() {
  int max_file_length = 100;
  struct file_system A;
  
  strncpy(A.inode_list[0].file_name,"taco.txt",99);
  A.inode_list[0].file_name[max_file_length-1]='\0';
  
  strncpy(A.inode_list[997].file_name,"test2.txt",99);
  A.inode_list[997].file_name[max_file_length-1]='\0';
  
  printf("%lu\n",sizeof(struct inode));
  printf("%lu\n",sizeof(struct file_system));
  FILE * file= fopen("output", "wb+");
  if (file != NULL) {
    fwrite(&A, sizeof(struct file_system), 1, file);
    //fclose(file);
  }
  struct file_system *B=malloc(sizeof(struct file_system));
  //file= fopen("output", "rb");
  rewind(file); 
  if (file != NULL) {
    fread(B, sizeof(struct file_system), 1, file);
    fclose(file);
  }
  
  printf("%s\n",B->inode_list[0].file_name); 
  printf("%s\n",B->inode_list[997].file_name);
}
