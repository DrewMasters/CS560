/******************
 *   CS 560- PA #1
 *   exec.c
 *   By: Joe Dorris
 *   Drew Masters
 ******************/

#include <stdlib.h>
#include <string.h>
#include "file_system.h"
#include "commands.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

extern "C" void fs_try_exec(FILE * fp, struct file_system * F, const char *filename) {
	FILE * exec_file;
	int fd;
	struct directory dir;
	int i;
	int flag;
	char * buffer;

	fseek(fp, F->inode_list[F->cur_idx].direct[0], SEEK_SET);
	fread(&dir, 1, sizeof(struct directory), fp);
	rewind(fp);

	flag = 1;
	for(i=0;i<MAX_SIZE_DIRECTORY;i++) {
		if(0==strcmp(filename,dir.files[i])) {
			if( 1 ) { //F->inode_list[dir.inodes[i]].file_type == 2 ) {
				//read in size of file
				flag = 0;
				fd = fs_open(fp,F,filename,"r");
				buffer = (char *)malloc(F->fd[fd].i->size);
				buffer = fs_read(fp,F,fd,F->fd[fd].i->size);
				fs_close(fp,F,fd);
				
				//open new file for executing
				exec_file = fopen("exec_temp","wb");
				//write memory to it
				fwrite(buffer,F->fd[fd].i->size,1,exec_file);
				fclose(exec_file);
				chmod("exec_temp",0777);

				//fork 
				pid_t parent = getpid();
				pid_t pid = fork();

				if (pid == -1)
				{
					// error, failed to fork()
				} 
				else if (pid > 0)
				{
					//wait on the child, then delete the file
					int status;
					waitpid(pid, &status, 0);
					unlink("exec_temp");
				}
				else 
				{
					// execute if child
					execve("./exec_temp",NULL,NULL);
					exit(1);
				}
			}
			else {
				printf("Not executable\n");
			}

		}
	}
	//there was no command or executable file found
	if(flag) printf("-sh: %s: command not found\n",filename);
}
