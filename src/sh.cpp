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
#include <string.h>
#include <algorithm>

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

	//set name for file system storage file
	if(argc>2) exit(1);
	else if(argc==2) fs_fname = argv[1];
	else fs_fname = "disk.disk";

	//allocate memory for file system
	F = (struct file_system*)malloc(sizeof(struct file_system));

	//open the file system file or create it
	if( access( fs_fname.c_str(), F_OK ) == 0 ) {
		fp = fopen(fs_fname.c_str(), "rb+");
	}
	else {
		fp = fopen(fs_fname.c_str(), "wb+");
	}
	rewind(fp);
	//printf("fp=%p\n",fp);
	//Try to read in the file system struct 
	if(fp != NULL) {
		//open it and read in if exists
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
		rewind(fp);
	} else {
		printf("file could not be opened\n");
		return 1;
	}


	/********************************************************
	 * Print prompt until exit.
	 *******************************************************/
	cout << prompt;
	while(1) {
		//Get input from user after prompt
		getline(cin,temp);
		istringstream iss(temp);
		iss >> command;
	    /********************************************************
	    * Check the command and do the specified action.
	    *******************************************************/
		if(command=="mkfs") {
			//Make new file system
			fs_mkfs(fp, F);
		}
		else if(command == "cd") {
			//Get location to cd to and call fs_cd
			iss >> arg1;
			fs_cd(F,fp,arg1.c_str());
		}
		else if(command == "mkdir") {
			//Get directory name
			iss >> arg1;
			//Save current directory location
			save = F->cur_idx;
			//cd to folder where want to make directory
			strcpy(temp_str,arg1.c_str());
			c = strrchr(temp_str,'/');
			if(c != NULL) {
				*c = '\0';
				fs_cd(F,fp,temp_str);
				c++;
			}
			else c = temp_str;

			//Make directory
			fs_mkdir(fp,F,c);
			//Return to cur directory
			F->cur_idx = save;
		}
		else if(command == "ls") {
			//list files in current directory
			fs_ls(fp,F);
		}
		else if(command=="rmdir") {
			iss >> arg1;
			//Go to directory to remove.
			save = F->cur_idx;
			strcpy(temp_str,arg1.c_str());
			c = strrchr(temp_str,'/');
			if(c != NULL) {
				*c = '\0';
				fs_cd(F,fp,temp_str);
				c++;
			}
			else c = temp_str;
			//Remove directory.
			fs_rmdir(fp,F,c);
			F->cur_idx = save;
		}
		else if(command=="open") {
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
			//open file
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
			//close file descriptor
			iss >> a1;
			fs_close(fp,F,a1);
		}
		else if(command=="read") {
			//read from file descirptor
			iss >> a1 >> a2;
			cout << fs_read(fp,F,a1,a2) << endl;
		}
		else if(command=="write") {
			string temp_str;
			//read in all arguments after file descriptor
			iss >> a1 >> arg1;
			while( iss >> temp_str ) {
				arg1 += " " + temp_str;
			}
			//remove quotes
			arg1.erase(remove(arg1.begin(), arg1.end(), '"'), arg1.end());
			//write to file descriptor
			fs_write(fp,F,a1,arg1.c_str(),strlen(arg1.c_str()));
		}
		else if(command=="seek") {
			iss >> a1 >> a2;
			fs_seek(fp,F,a1,a2);
		}
		else if(command=="cat") {
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
			save = F->cur_idx;
			strcpy(temp_str,command.c_str());
			c = strrchr(temp_str,'/');
			if(c != NULL) {
				*c = '\0';
				fs_cd(F,fp,temp_str);
				c++;
			}
			else c = temp_str;
			fs_try_exec(fp,F, c);
			F->cur_idx = save;
		}
		cout << prompt;
	}

	//if file is empty delete file

	free(F);
	fclose(fp);
	return 0;
}
