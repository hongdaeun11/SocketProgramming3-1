// echo_server_fork.c
#include        <stdio.h>
#include        <stdlib.h>
#include        <string.h>
#include        <unistd.h>
#include        <netinet/in.h>
#include        <sys/socket.h>

#define         PORT    9019

void do_echo(int);

main()
{
        int     connSock, listenSock;
        struct  sockaddr_in s_addr, c_addr;
        int     len;

        pid_t   pid;

        listenSock = socket(PF_INET, SOCK_STREAM, 0);

        memset( &s_addr, 0, sizeof(s_addr) );
       	s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
       	s_addr.sin_family = AF_INET;
       	s_addr.sin_port = htons(PORT);

        if (bind(listenSock, (struct sockaddr *)&s_addr, sizeof(s_addr)) == -1) {
                printf("Can not Bind\n");
                return -1;
        }

        if(listen(listenSock, 5) == -1) {
                printf("listen Fail\n");
                return -1;
        }

        while(1) {

               len = sizeof(c_addr);
               connSock = accept(listenSock, (struct sockaddr *)&c_addr, &len);
	       printf("connSocket is accepted!... socket number : %d....\n", connSock);

               if ( (pid = fork()) < 0 ) { 	
                       printf("echo server can not fork()\n");
                       return -1;
               } else if ( pid > 0 ) { 	
                       close(connSock);
                       continue;
               } else if ( pid == 0 ) { 
                       close(listenSock);
                       do_echo(connSock);
               }
       }
}

void
do_echo(int connSock) {
       int     n;
       char    rcvBuffer[BUFSIZ];

       while( (n = read(connSock, rcvBuffer, sizeof(rcvBuffer))) != 0) {
               printf("child process is completed \n");
               write(connSock, rcvBuffer, n);
       }
       exit(0);
}
