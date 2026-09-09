#include        <stdio.h>
#include        <stdlib.h>
#include        <string.h>
#include        <unistd.h>
#include        <netinet/in.h>
#include        <sys/socket.h>
#include        <pthread.h>

#define         PORT    9019

void* do_echo(void *);

main()
{
        int     connSock, listenSock;
        struct  sockaddr_in s_addr, c_addr;
        int     len;

        pthread_t       pthread1;
        int             thr_id;

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

                thr_id = pthread_create(&pthread1, NULL, do_echo, (void *)connSock);
        }
}

void*
do_echo(void* data) 
{
        int     n;
        char    rcvBuffer[BUFSIZ];

        int     connSock = (int)data;

        while( (n = read(connSock, rcvBuffer, sizeof(rcvBuffer))) != 0) {
                write(connSock, rcvBuffer, n);
        }
}

