// remote_addr.c
#include        <stdio.h>
#include        <sys/socket.h>
#include        <netinet/in.h>
#include	<stdlib.h>

main(int argc, char* argv[])
{
        int     tcpSd;
        struct sockaddr_in s_addr;
	char *ip_str = argv[1];
	int port = atoi(argv[2]);

        if ( (tcpSd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
                perror("socket()");
                exit (-1);
        }

	unsigned long ip = inet_addr(ip_str);
	unsigned short port_input = htons(port);

        bzero( (char *)&s_addr, sizeof(s_addr) );    	
        s_addr.sin_family = AF_INET;		
        inet_aton("203.249.39.3",&s_addr.sin_addr.s_addr);
        s_addr.sin_port = htons(7);			

        printf("ip = %s (binary)=%x\n", ip_str,ntohl(s_addr.sin_addr.s_addr));
        printf("port no =%x\n", port_input);
	printf("port no =%x\n", ntohs(port_input));

        close(tcpSd);
}
