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
#include <jrb.h>
#include <sys/wait.h>

main(int argc, char *argv[]) {
	IS is;
	char * prompt;
	char * command;
	int status;
	char ** newargv;
	int arg_it;
	int i, j;
	int no_wait = 0;
	int newargc;
	int NF;
	int word_len;

	int field_it;
	int pid, wait_ret;

	int fd1, fd2;
	int pipefd[2][2];

	JRB children, node;
	int finished;

	int first, last, start, end;

	/*****************************************************************
	  Set prompt
	 ******************************************************************/
	if(argc>2) {
		fprintf(stderr, "Usage: jsh3 prompt\n");
	}
	else if(argc==2) {
		if(argv[1][0]=='-') prompt="";
		else prompt = argv[1];
	}
	else {
		prompt = "jsh3:";
	}

	/*****************************************************************
	  Allocate memory
	 ******************************************************************/
	command = malloc(sizeof(char)*100001);
	newargv = (char**)malloc(sizeof(char*)*100);
	for(i=0;i<100;i++) {
		newargv[i] = (char*)malloc(sizeof(char)*1000);
	}
	is = new_inputstruct(NULL);

	/*****************************************************************
	  Provide prompt for shell commands until EOF or exit
	 ******************************************************************/
	printf("%s", prompt);
	while(get_line(is)>0) {
		//check for exit
		if(strcmp(is->fields[0],"exit")==0) exit(0);

		//make a new tree for holding the children pids
		children = make_jrb();
		//set variables to initial values
		no_wait=0;
		NF=is->NF;
		field_it=0;
		word_len = strlen(is->fields[NF-1]);
		//check for & to see if you should wait
		if(is->fields[NF-1][word_len-1]=='&') {
			no_wait=1;
			//remove the & from the fields
			if(word_len==1) NF--; 
			else is->fields[NF-1][word_len-1]='\0';
		}

		//if(is->fields[NF-1][0]=='\0') NF=NF-1;


		//cycle through the fields
		while(field_it<NF) {
			//set that it is not first or last process
			first=0;
			last=0;

			//see if it is the first or last process
			start = field_it;
			for(i=field_it;i<NF;i++) {
				if(i==0) first=1;
				if(i==NF-1) last = 1;
				//check for a pipe
				if(is->fields[i][0]=='|') {
					if(i==0 || i==NF-1) {
						fprintf(stderr,"Misplaced |\n");
						exit(1);
					}
					break;
				}
			}
			//set where to continue cycling through fields
			field_it=i+1;
			//set the end of the arguments
			end = i-1;

			//if not the last process then a pipe is needed
			if(!last) {
				i = pipe(pipefd[1]);
				if (i < 0) {
					perror(is->fields[start]);
					exit(1);
				}
			}

			//fork for the current process
			pid=fork();
			
			/*****************************************************************
				Child will set args and perform file redirection then exit
			******************************************************************/
			if(pid==0) {
				arg_it=0;
				argc=0;
				if(!first) {
					//redirect input
					if (dup2(pipefd[0][0], 0) != 0) {   
						perror("Error: dup2(pipefd[0])");
						exit(1);
					}
					//close the read end of previous process
					close(pipefd[0][0]);
				}
				if(!last) {
					//redirect output
					if (dup2(pipefd[1][1], 1) != 1) {   
						perror("Error: dup2(pipefd[1])");
						exit(1);
					}
					//close both ends of new pipe
					close(pipefd[1][0]);
					close(pipefd[1][1]);
				}  
				//cycle through fields building up arguments
				for(i=start;i<=end;i++) {
					//check for all file redirection
					if(is->fields[i][0]=='<') {
						fd1 = open(is->fields[i+1], O_RDONLY);
						if (dup2(fd1, 0) != 0) {
							perror("Error: dup2()");
							exit(1);
						}
						close(fd1);
						i+=1;
					}
					else if(is->fields[i][0]=='>') {
						if(strlen(is->fields[i])>1) {
							fd2 = open(is->fields[i+1], O_WRONLY | O_APPEND | O_CREAT, 0644);
						}
						else {
							fd2 = open(is->fields[i+1], O_WRONLY | O_TRUNC | O_CREAT, 0644);
						}
						if (dup2(fd2, 1) != 1) {
							perror("Error: dup2()");
							exit(1);
						}
						close(fd2);
						i+=1;
					}
					//if not a redirection then add to argument list
					else {
						newargv[arg_it]=is->fields[i];
						newargc++;
						arg_it++;
					}
				}

				//execute the program
				newargv[newargc]=NULL;
				execvp(newargv[0], newargv);
				perror(newargv[0]);
				exit(1);
			}
			
			/*****************************************************************
				Parent will continue while loop until finishing the whole line
			******************************************************************/
			else{
				if(!first) {
					//close read pipefd from previous process 
					close(pipefd[0][0]);
				}
				if(!last) {
					//close write pipefd from previous process
					close(pipefd[1][1]);
					//move over the read end for the next process to use
					pipefd[0][0]=pipefd[1][0];
				}
				//add the pid to the tree
				jrb_insert_int(children,pid,new_jval_i(1));
				//if the last argument was used then break
				if(last) {
					break;
				}
			}
		}

		/*****************************************************************
			Parent process will wait if necessary then output the prompt
		******************************************************************/
		// if you are supposed to wait
		if(!no_wait) {
			//keep checking the return value
			wait_ret = wait(&status);
			while(wait_ret!=-1) {
				finished = 1;
				//set the value in the tree for this pid to 0
				node = jrb_find_int(children, wait_ret);
				if (node != NULL) {
					node->val.i=0;
				}
				//if any 1's remain than set the finished flag to false
				jrb_traverse(node, children) {
					if(node->val.i==1) {
						finished=0;
					}
				}
				//if finished break
				if(finished) {
					jrb_free_tree(children);
					break;
				}
				//else wait again
				wait_ret = wait(&status);
			}
		}
		//free the tree if not supposed to wait
		else {
			jrb_free_tree(children);
		}
		//resend the prompt
		printf("%s", prompt);
	}
	exit(0);
}
