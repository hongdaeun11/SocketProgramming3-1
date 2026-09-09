#include        <stdio.h>
#include        <sys/socket.h>
#include        <netinet/in.h>
#include	<stdlib.h>

main()
{
   int tcpSd;
   struct sockaddr_in s_addr;
   char *IPaddr = "192.168.0.255";
   if ( (tcpSd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
	perror("socket()");
	exit (-1);
   }
   bzero( (char *)&s_addr, sizeof(s_addr) );
   s_addr.sin_family = AF_INET;
   inet_aton(IPaddr, &s_addr.sin_addr.s_addr);
   s_addr.sin_port = htons(7);
   printf("1. ip =%s\n", IPaddr);
   printf("2. ip(binary)=%x\n",ntohl(s_addr.sin_addr.s_addr));
   printf("3. port no =%d\n", ntohs(s_addr.sin_port));
   printf("4. port no =%d\n", s_addr.sin_port);
   close(tcpSd);
}
