/******************
  CS 560- PA #1
  Basic shell
  By: Joe Dorris
      Drew Masters
 *****************/

#include <iostream>
#include <sstream>
#include "commands.h"
#include "file_system.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

int main(int argc, char *argv[]) {
  string prompt = "sh: ";
  struct file_system * F;
  string temp;
  string command;
  string arg1;
  string fs_fname;
  FILE * fp;

  if(argc>2) exit(1);
  else if(argc==2) fs_fname = argv[1];
  else fs_fname = "disk.disk";
    
  F = (struct file_system*)malloc(sizeof(struct file_system));

  fp = fopen(fs_fname.c_str(), "rb+");
  printf("fp=%p\n",fp);
  if(fp != NULL) {
  //open it and read in if exists
  if( access( fs_fname.c_str(), F_OK ) == 0 ) {
    // file exists
    // read it into F
    int bytes_read = fread(F, 1, sizeof(struct file_system), fp);
    printf("read in %d bytes\n", bytes_read);
    if( sizeof(struct file_system) != bytes_read) {
      printf("Could not read it in\n");
      free(F);
      F = (struct file_system*)malloc(sizeof(struct file_system));
      fclose(fp);
      fp = fopen(fs_fname.c_str(), "wb+");
    }
    else {
      F->root_idx = 0;
      F->cur_idx = 0;
    }
  } else {
      printf("Could not access\n");
  }
  rewind(fp);
  } else {
    printf("file could not be opened\n");
    return 1;
  }
  

  cout << prompt;
  while(1) {
    getline(cin,temp);
    istringstream iss(temp);
    iss >> command;
    if(command=="mkfs") {
      //cout << "making file system" << endl;
      //clear file
      //free file_system struct
      fs_mkfs(fp, F);
    }
    else if(command == "cd") {
      //need to get second word
      cout << "got a cd" << endl;
      iss >> arg1;
      fs_cd(F,fp,arg1.c_str());
    }
    else if(command == "ls") {
      fs_ls(fp,F);
    }
    else if(command=="exit") {
      break;
    }
    else {
      cout << "Unrecognized" << endl;
    }
    cout << prompt;
  }

  //if file is empty delete file

  free(F);
  fclose(fp);
  return 0;
}
