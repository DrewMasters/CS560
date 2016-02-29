/******************
  CS 560- PA #1
  rmdir.c
By: Joe Dorris
Drew Masters
 *****************/

#include <stdlib.h>
#include <string.h>
#include "file_system.h"
#include "commands.h"
#include <unistd.h>

extern "C" void fs_rmdir(FILE * fp, struct file_system * F, const char *dirname){
	//remove sub-directory called dirname if it is empty
	//free inode and directory
	int i;
	int flag=1;
	struct directory dir, dir2;

	fseek(fp,F->inode_list[F->cur_idx].direct[0], SEEK_SET);
	fread(&dir, 1, sizeof(struct directory), fp);
	rewind(fp);
	int r;

	if (!(strcmp(dirname,".")||strcmp(dirname,".."))){
		return;
	}

	for(i=0;i<MAX_SIZE_DIRECTORY;i++) {
		r=strcmp(dirname,dir.files[i]);
		if(0==r) {
			/*if found set flag to zero and check to see if inode file type is directory or not.
			  if inode file type is directory set file systems current directory to this directory's inode.*/
			flag = 0; 
			if( F->inode_list[dir.inodes[i]].file_type == 1 ) {
				fseek(fp,F->inode_list[dir.inodes[i]].direct[0], SEEK_SET);
				fread(&dir2, 1, sizeof(struct directory), fp);
				rewind(fp);
				if( dir2.files[2][0]=='\0' ){
					//free page
					free_page(F, F->inode_list[dir.inodes[i]].direct[0]);
					//set inode as not in use
					F->inode_list[dir.inodes[i]].in_use=0;
					//remove directory name from current directory
					dir.files[i][0]='\0';
					//write current directory back to file
					fseek(fp,F->inode_list[F->cur_idx].direct[0], SEEK_SET);
					fwrite(&dir, sizeof(struct directory),1,fp);
					rewind(fp);
				} else {
					printf("Directory not empty\n");
				}
			} else {
				printf("Not a directory\n");
			}
			break;
		}
	}
	if(flag) printf("Directory not found\n");
	rewind(fp);

	return;
}
