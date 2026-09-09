#include        <stdio.h>
#include        <netinet/in.h>
#include        <sys/socket.h>

#define         PORT    9019

char    buffer[BUFSIZ] = "hello, message from port 9019";
char	rBuffer[BUFSIZ];

main()
{
        int     c_socket, s_socket;
        struct  sockaddr_in s_addr, c_addr;
        int     len;
        int     n, i;

	char	*temp;
	int	length;

        s_socket = socket(PF_INET, SOCK_STREAM, 0);
	printf("1. Server socket is on....(%d)\n", s_socket);


        memset( &s_addr, 0, sizeof(s_addr) );
        s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        s_addr.sin_family = AF_INET;
        s_addr.sin_port = htons(PORT);

        if (bind(s_socket, (struct sockaddr *)&s_addr, sizeof(s_addr)) == -1) {
                printf("Can not Bind\n");
                return -1;
        }
	printf("2. Socket is binded to port 9019....\n");

        if(listen(s_socket, 5) == -1) {
                printf("listen Fail\n");
                return -1;
        }

        while(1) {
                len = sizeof(c_addr);
                c_socket = accept(s_socket, (struct sockaddr *)&c_addr, &len);

		length = 0;
		temp = rBuffer;
		while ((n = read(c_socket, temp, 1)) > 0) {
			if (*temp == '\r') continue;
			if (*temp == '\n') break;
			if (*temp == '\0') break;
 
     			if (length == BUFSIZ) break;
     			temp++; length++;
     		}

		rBuffer[length] = '\0';

		if ( !strcmp(rBuffer, "print") ) {
        		n = strlen(buffer);
                	write(c_socket, buffer, n);
		}
                close(c_socket);
        }
        close(s_socket);
}
