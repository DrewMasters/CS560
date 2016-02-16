/******************
  CS 560- PA #1
  Basic shell
  By: Joe Dorris
      Drew Masters
 *****************/

#include <iostream>
#include "commands.h"
#include "file_system.h"
#include <unistd.h>

using namespace std;

int main(int argc, char *argv[]) {
  string prompt = "sh: ";
  struct file_system * F;
  string temp;
  string fs_fname;
  FILE * fp;

  if(argc>2) exit(1);
  else if(argc==2) fs_fname = argv[1];
  else fs_fname = "disk.disk";
    
  F = (struct file_system*)malloc(sizeof(struct file_system));

  //open it and read in if exists
  if( access( fs_fname.c_str(), F_OK ) != -1 ) {
    // file exists
    fp = fopen(fs_fname.c_str(), "rb+");
    // read it into F
    if( sizeof(struct file_system) != fread(F, sizeof(struct file_system), 1, fp)) {
      free(F);
      F = (struct file_system*)malloc(sizeof(struct file_system));
      fclose(fp);
      fp = fopen(fs_fname.c_str(), "wb+");
      rewind(fp);
    }
  } else {
    // file doesn't exist
    fp = fopen(fs_fname.c_str(), "wb+");
  }
  

  cout << prompt;
  while(cin >> temp) {
    if(temp=="mkfs") {
      //cout << "making file system" << endl;
      //clear file
      //free file_system struct
      fs_mkfs(fp, F);
    }
    else if(temp == "cd") {
      //need to get second word
      cout << "got a cd" << endl;
    }
    else if(temp=="exit") {
      break;
    }
    else {
      cout << "Unrecognized" << endl;
    }
    cout << prompt;
  }
  free(F);
  fclose(fp);
  return 0;
}
