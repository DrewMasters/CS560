#include <stdlib.h>
#include <string.h>
#include "file_system.h"
#include "commands.h"

extern "C" void fs_try_exec(FILE * fp, struct file_system * F, const char *filename) {

	FILE * exec_file;

	fseek(fp, F->inode_list[F->cur_idx].direct[0], SEEK_SET);
	fread(&dir, 1, sizeof(struct directory), fp);
	rewind(fp);
	int r;

	void * buffer;

	for(i=0;i<MAX_SIZE_DIRECTORY;i++) {
		if(0==strcmp(dirname,dir.files[i])) {
			if( F->inode_list[dir.inodes[i]].file_type == 2 ) {
				//read in size of file
				buffer = malloc(F->fd[fd].i->size);
				fd = fs_open(fp,F,filename,&flag);
				//buffer = fs_read(fp,F,fd,F->fd[fd].i->size)
				//open new file for executing
				exec_file = fopen("exec_temp","wb");
				//write memory to it
				fwrite(buffer,F->fd[fd].i->size,1,exec_file);
				fclose(exec_file);

				//fork 
				pid_t parent = getpid();
				pid_t pid = fork();

				if (pid == -1)
				{
					// error, failed to fork()
				} 
				else if (pid > 0)
				{
					int status;
					waitpid(pid, &status, 0);
				}
				else 
				{
					// we are the child
					execve("./exec_temp");
					//_exit(EXIT_FAILURE);   // exec never returns
				}
			}
			else {
				printf("Not executable\n")
			}

		}
	}
	if(flag) printf("Unrecognized command\n");
}
