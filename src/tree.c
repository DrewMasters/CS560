/******************
  CS 560- PA #1
  Basic shell
By: Joe Dorris
    Drew Masters
 *****************/

#include <stdlib.h>
#include <string.h>
#include "file_system.h"
#include "commands.h"
#include <unistd.h>

extern "C" void fs_tree(FILE * fp, struct file_system * F, int tl){
	//list contents of current directory in tree-format
	//for each file date file size should be included?

	int i,j;
	struct directory dir;
	fseek(fp, F->inode_list[F->cur_idx].direct[0], SEEK_SET);
	fread(&dir, 1, sizeof(struct directory), fp);
	rewind(fp);

	//iterate through current directory
	for(i=0;i<MAX_SIZE_DIRECTORY;i++) {
		if(dir.files[i][0] != '\0') {
			for (j=0; j<tl; j++) printf("  "); //prints out current indentation level  
			if (F->inode_list[dir.inodes[i]].file_type == 0 || strcmp(".", dir.files[i])==0 || strcmp("..",dir.files[i])==0){
				//if file is a regular file print out name and size
				//if file is one of the first two directories than just print the name
				if (strcmp(".",dir.files[i])==0|| strcmp("..",dir.files[i])==0) printf("%s\n",dir.files[i]);
				else printf("%s %d\n",dir.files[i],F->inode_list[dir.inodes[i]].size);
			} else{
				//print out directory name
				printf("%s\n", dir.files[i]);
				//cd into child directory
				fs_cd(F, fp, dir.files[i]);
				//call fs_tree on child directory
				fs_tree(fp, F, tl+1);
				//when it returns cd back into parent directory
				fs_cd(F, fp, "..");
			}
		}
	}
	return;
}
