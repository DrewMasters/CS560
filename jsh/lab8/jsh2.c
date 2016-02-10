/******************
  CS 360- Lab 8
By: Joe Dorris
 *****************/

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <fields.h>
#include <fcntl.h>
#include <sys/resource.h>

main(int argc, char *argv[]) {
	IS is;
	char * prompt;
	char * command;
	int status;
	char ** newargv;
	int arg_it;
	int word_start, c_it;
	int i;
	int no_wait = 0;
	int newargc;
	int NF;
	int word_len;

	int pid, wait_ret;
	int fd1, fd2;

	/*****************************************************************
	  Set prompt
	 ******************************************************************/
	if(argc>2) {
		fprintf(stderr, "Usage: jsh1 prompt\n");
	}
	else if(argc==2) {
		if(argv[1][0]=='-') prompt="";
		else prompt = argv[1];
	}
	else {
		prompt = "jsh2:";
	}

	/*****************************************************************
	  Allocate memory
	 ******************************************************************/
	command = malloc(sizeof(char)*1001);
	newargv = (char **) malloc(sizeof(char *)*501);
	is = new_inputstruct(NULL);

	/*****************************************************************
	  Provide prompt for shell commands until EOF or exit
	 ******************************************************************/
	printf("%s", prompt);
	while(get_line(is)>0) {
		if(strcmp(is->fields[0],"exit")==0) exit(0);
		no_wait=0;
		NF=is->NF;
		newargc=0;

		//check for & to know if the parent should wait
		word_len = strlen(is->fields[NF-1]);
		if(is->fields[NF-1][word_len-1]=='&') {
			no_wait=1;
			if(word_len==1) NF--; 
			else is->fields[NF-1][word_len-1]='\0';
		}

		//create child process and execute
		pid = fork();
		if(pid == 0) {
			for(i=0;i<NF;i++) {
				//set file redirection
				if(is->fields[i][0]=='<') {
					fd1 = open(is->fields[i+1], O_RDONLY);
					dup2(fd1, 0);
					close(fd1);
					i+=1;
				}
				else if(is->fields[i][0]=='>') {
					if(strlen(is->fields[i])>1) {
						fd2 = open(is->fields[i+1], O_WRONLY | O_APPEND | O_CREAT, 0644);
						dup2(fd2, 1);
						close(fd2);
					}
					else {
						fd2 = open(is->fields[i+1], O_WRONLY | O_TRUNC | O_CREAT, 0644);
						dup2(fd2, 1);
						close(fd2);
					}
					i+=1;
				}
				else {
					newargv[i]=is->fields[i];
					newargc++;
				}

			}
			//set final argument
			newargv[newargc]=NULL;

			//execute
			execvp(newargv[0], newargv);
			perror(newargv[0]);
			exit(1);
		}
		/*****************************************************************
		  Parent process will wait if necessary then output the prompt
		 ******************************************************************/
		else {
			if(!no_wait) {
				wait_ret = wait(&status);
				while(wait_ret != pid && wait_ret != -1) {
					wait_ret = wait(&status);
				}
			}
			else {
			}
		}
		printf("%s", prompt);
	}

	exit(0);
}
