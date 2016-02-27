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
#include <string>

using namespace std;

int main(int argc, char *argv[]) {
	string prompt = "sh: ";
	struct file_system * F;
  int save; 
	string temp;
	string command;
	string arg1,arg2;
	string fs_fname;
	FILE * fp;
	int a1,a2;
  int ret_fd;
  char temp_str[100];
  char * c;

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
		//NEED TO RESET ARGS
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
		else if(command == "mkdir") {
			//need to get second word
			cout << "got a mkdir" << endl;
			iss >> arg1;
      save = F->cur_idx;
      strcpy(temp_str,arg1.c_str());
      c = strrchr(temp_str,'/');
      if(c != NULL) {
        *c = '\0';
        fs_cd(F,fp,temp_str);
        c++;
      }
      else c = temp_str;
      
			fs_mkdir(fp,F,c);
      F->cur_idx = save;
		}
		else if(command == "ls") {
			fs_ls(fp,F);
		}
		else if(command=="rmdir") {
			cout << "got a rmdir" << endl;
			iss >> arg1;
      save = F->cur_idx;
      strcpy(temp_str,arg1.c_str());
      c = strrchr(temp_str,'/');
      if(c != NULL) {
        *c = '\0';
        fs_cd(F,fp,temp_str);
        c++;
      }
      else c = temp_str;
			fs_rmdir(fp,F,c);
      F->cur_idx = save;
		}
		else if(command=="open") {
			cout << "open" << endl;
			iss >> arg1 >> arg2;
      save = F->cur_idx;
      strcpy(temp_str,arg1.c_str());
      c = strrchr(temp_str,'/');
      if(c != NULL) {
        *c = '\0';
        fs_cd(F,fp,temp_str);
        c++;
      }
      else c = temp_str;
      ret_fd = fs_open(fp,F,c,arg2.c_str());
      if( -1 != ret_fd ) {
        cout << "SUCCESS,fd=" << ret_fd  << endl;
      }
      else {
        cout << "error, no fd available" << endl;
      }
      F->cur_idx = save;
		}
		else if(command=="close") {
			cout << "close" << endl;
			iss >> arg1;
			fs_close(fp,F,stoi(arg1));
		}
		else if(command=="read") {
			cout << "read" << endl;
			iss >> a1 >> a2;
			cout << fs_read(fp,F,a1,a2) << endl;
		}
		else if(command=="write") {
      string temp_str;
			cout << "write" << endl;
			iss >> a1 >> arg1;
      while( iss >> temp_str ) {
        arg1 += " " + temp_str;
      }
      cout << arg1 << endl;
      arg1.erase(std::remove(arg1.begin(), arg1.end(), '"'), arg1.end());
      cout << arg1 << endl;
			fs_write(fp,F,a1,arg1.c_str());
		}
		else if(command=="seek") {
			cout << "seek" << endl;
			iss >> a1 >> a2;
			fs_seek(fp,F,a1,a2);
		}
		else if(command=="cat") {
			cout << "cat" << endl;
			iss >> arg1;
      save = F->cur_idx;
      strcpy(temp_str,arg1.c_str());
      c = strrchr(temp_str,'/');
      if(c != NULL) {
        *c = '\0';
        fs_cd(F,fp,temp_str);
        c++;
      }
      else c = temp_str;
			fs_cat(fp,F,c);
      F->cur_idx = save;
		}
		else if (command=="tree"){
			fs_tree(fp,F,0);
		}
		else if(command=="export") {
			cout << "export" << endl;
			iss >> arg1 >> arg2;
      save = F->cur_idx;
      strcpy(temp_str,arg1.c_str());
      c = strrchr(temp_str,'/');
      if(c != NULL) {
        *c = '\0';
        fs_cd(F,fp,temp_str);
        c++;
      }
      else c = temp_str;
			fs_export(fp,F,c,arg2.c_str());
      F->cur_idx = save;
		}
		else if(command=="import") {
			cout << "import" << endl;
			iss >> arg1 >> arg2;
      save = F->cur_idx;
      strcpy(temp_str,arg2.c_str());
      c = strrchr(temp_str,'/');
      if(c != NULL) {
        *c = '\0';
        fs_cd(F,fp,temp_str);
        c++;
      }
      else c = temp_str;
			fs_import(fp,F,arg1.c_str(),c);
      F->cur_idx = save;
		}
		else if(command=="exit") {
      fs_exit(fp,F);
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
