//ls_client.c
#include        <stdio.h>
#include        <netinet/in.h>
#include        <sys/socket.h>
#include        <strings.h>

#define         PORT            9019
#define         IPADDR          "127.0.0.1"

char	buffer[BUFSIZ];

main(int argc, char *argv[])
{
        int     c_socket;
        struct  sockaddr_in c_addr;
        int     len;

        char    rcvBuffer[BUFSIZ];

	char	*temp;
	int	length = 0;

        int     n;
	int 	count = 0;

        c_socket = socket(PF_INET, SOCK_STREAM, 0);

        memset( &c_addr, 0, sizeof(c_addr) );
        c_addr.sin_addr.s_addr = inet_addr(IPADDR);
        c_addr.sin_family = AF_INET;
        c_addr.sin_port = htons(PORT);

        if (connect(c_socket, (struct sockaddr *)&c_addr, sizeof(c_addr)) == -1) {
                printf("Can not connect\n");
                close(c_socket);
                return -1;
        }


	printf("Input command\n\n");
	scanf("%s", buffer);
	buffer[strlen(buffer)] = '\0';
	if ((n = write(c_socket, buffer, strlen(buffer)+1)) < 0) {
		printf("write error\n");
		exit(-1);
	}

	temp = rcvBuffer;
	while ((n = read(c_socket, temp, 1)) > 0) {
		if (length == BUFSIZ) break;
		temp++; length++;
	}

	rcvBuffer[length] = '\0';

        printf("received Data : %s\n", rcvBuffer);
	//printf("파일의 개수 총 %d 개 \n", );

        close(c_socket);
}
