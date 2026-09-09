// stdio.c
#include        <stdio.h>
#include        <sys/socket.h>

main() {
	int     sd;
	char str[100];

	sd = socket(PF_INET, SOCK_STREAM, 0);

	fscanf(stdin, "%s", str);            
	fprintf(stdout, "%s \n", str);         
	fprintf(stderr, "sd = [%d] \n", sd);
}
