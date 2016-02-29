#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sockettome.h"

main(int argc, char **argv)
{
	char *hn, *un;
	int port, fd;
	char s[1000];
	FILE *fin, *fout;
	int num_read;

	if (argc != 3) { fprintf(stderr, "usage: client1 hostname port\n"); exit(1); }

	hn = argv[1];
	port = atoi(argv[2]);
	un = getenv("USER");

	fd = request_connection(hn, port);

	fin = fdopen(fd, "r+");
	//fout = fdopen(fd, "w");

	/*if (dup2(0,fileno(fin)) != fileno(fin)) {
		perror("Error: dup2(0) failed");
		exit(1);
	}*/

	//while(1) {
		//while(1) {
		num_read = fread(s,999,1,fin);

		s[num_read]='\0';
		printf("%s", s);
		//  if(num_read >= 999) break;
		//}

		if (fgets(s, 1000, stdin) == NULL) {
			printf("Server closed prematurely\n");
			exit(0);
		}
		fprintf(fout, "%s", s);
	//}
	exit(0);
}
