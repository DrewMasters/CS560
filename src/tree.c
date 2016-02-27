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

	for(i=0;i<MAX_SIZE_DIRECTORY;i++) {
		if(dir.files[i][0] != '\0') {
			//printf("%d %s\n",tl,dir.files[i]);
			for (j=0; j<tl; j++) printf("  ");  
			if (F->inode_list[dir.inodes[i]].file_type == 0 || strcmp(".", dir.files[i])==0 || strcmp("..",dir.files[i])==0){
				if (strcmp(".",dir.files[i])==0|| strcmp("..",dir.files[i])==0) printf("%s\n",dir.files[i]);
				else printf("%s %d\n",dir.files[i],F->inode_list[dir.inodes[i]].size);
			} else{
					printf("%s\n", dir.files[i]);
					fs_cd(F, fp, dir.files[i]);
					fs_tree(fp, F, tl+1);
					fs_cd(F, fp, "..");
			}
		}
	}
	return;
}
