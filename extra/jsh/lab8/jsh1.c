/******************
  CS 360- Lab 8
By: Joe Dorris
 *****************/

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <fields.h>
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
	int word_len;
	struct rusage ru;
	int pid;
	int wait_ret;

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
		prompt = "jsh1:";
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
		newargc=is->NF;

		//check for & to know if the parent should wait
		word_len = strlen(is->fields[newargc-1]);
		if(is->fields[newargc-1][word_len-1]=='&') {
			no_wait=1;
			if(word_len==1) newargc--; 
			else is->fields[newargc-1][word_len-1]='\0';
		}

		//build up the arguments to execute
		for(i=0;i<newargc;i++) {
			newargv[i]=is->fields[i];
		}
		newargv[newargc]=NULL;

		//create child process and execute
		pid = fork();
		if(pid == 0) {
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
