#include <stdlib.h>
#include <stdio.h>
#include "sockettome.h"
#include <unistd.h>

int main() {
	int port,sock;
	FILE * fin, *fout;
	char s[1000];
	int pid;
	int fd;

	port = 5007;
	sock = serve_socket(port);

	fd = accept_connection(sock);

	fin = fdopen(fd, "r+");
	//fout = fdopen(fd, "w");

	//while(1) {
	pid = fork();
	if(pid==0) {
		if (dup2(fileno(fin), 0) != 0) {
			perror("Error: dup2(0) failed");
			exit(1);
		}
		if (dup2(fileno(fin), 1) != 1) {
			perror("Error: dup2(1) failed");
			exit(1);
		}

		//close the read end of previous process
		//close(fd);
		execl("/home/jdorris7/cs560/new_repo/CS560/bin/sh",NULL);//"../bin/sh");
		perror("messed up");
		exit(1);
	}
	else {
		waitpid(pid);
		exit(0);
		

		/*if(fgets(s,1000,fin)==NULL) {
			exit(1);
		}
		if(s[strlen(s)-1]=='\n') s[strlen(s)-1]='\0';



		fputs(output_line,fout)*/
	}
	//}
}
