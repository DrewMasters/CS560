#include <stdlib.h>
#include <string.h>
#include "file_system.h"
#include "commands.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

extern "C" void fs_try_exec(FILE * fp, struct file_system * F, const char *filename) {

	FILE * exec_file;
	int fd;
	struct directory dir;
	int r;
	int i;
	int flag;

	fseek(fp, F->inode_list[F->cur_idx].direct[0], SEEK_SET);
	fread(&dir, 1, sizeof(struct directory), fp);
	rewind(fp);

    printf("read in the directory\n");

	char * buffer;

	flag = 1;
	for(i=0;i<MAX_SIZE_DIRECTORY;i++) {
		if(0==strcmp(filename,dir.files[i])) {
			printf("%s\n",filename);
			if( 1 ) { //F->inode_list[dir.inodes[i]].file_type == 2 ) {
				//read in size of file
				flag = 0;
				fd = fs_open(fp,F,filename,"r");
				printf("size=%d\n",F->fd[fd].i->size);
				fflush(stdout);
				buffer = (char *)malloc(F->fd[fd].i->size);
				buffer = fs_read(fp,F,fd,F->fd[fd].i->size);
				printf("read into buffer\n");
				fflush(stdout);
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
					execve("./exec_temp",NULL,NULL);
					//_exit(EXIT_FAILURE);   // exec never returns
				}
			}
			else {
				printf("Not executable\n");
			}

		}
	}
	if(flag) printf("Unrecognized command\n");
}
