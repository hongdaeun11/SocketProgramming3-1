// ls_server.c
#include        <stdio.h>
#include        <netinet/in.h>
#include        <sys/socket.h>
#include        <dirent.h>

#define         PORT    9019

char    err_1[]="Directory Error";
char 	buffer[BUFSIZ];
char    rBuffer[BUFSIZ];  //receive buffer용도 

main()
{
        int     c_socket, s_socket;
        struct  sockaddr_in s_addr, c_addr;
        int     len, length;
        int     n, i;
	char	*temp;

	DIR *dp;
        struct dirent *dir;

        s_socket = socket(PF_INET, SOCK_STREAM, 0);
	printf("1. A server socket is created");
	printf("(socket number : %d)...\n" ,s_socket);

        memset( &s_addr, 0, sizeof(s_addr) );
        s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        s_addr.sin_family = AF_INET;
        s_addr.sin_port = htons(PORT);

        if (bind(s_socket, (struct sockaddr *)&s_addr, sizeof(s_addr)) == -1) {
                printf("Can not Bind\n");
                return -1;
        }
	printf("2. socket binding is completed...\n");


        if(listen(s_socket, 5) == -1) {
                printf("listen Fail\n");
                return -1;
        }
	printf("3. socket is now listening...\n");

        while(1) {
                len = sizeof(c_addr);
                c_socket = accept(s_socket, (struct sockaddr *)&c_addr, &len);
		printf("4. a new client socket is created");
		printf("(socket number : %d)...\n", c_socket);

                length = 0;
                temp = rBuffer;
                while ((n = read(c_socket, temp, 1)) > 0) {
                        if (*temp == '\r') continue;
                        if (*temp == '\n') break;
                        if (*temp == '\0') break;

                        if (length == BUFSIZ) break;
			printf("n=%d, length=%d, temp=%c\n", n, length, *temp);
                        temp++; length++;
                }
		printf("rBuffer : %s (length: %d)\n", rBuffer, length);
                rBuffer[length] = '\0';

		if( !strcmp(rBuffer, "print")){
			n = strlen(buffer);
			write(c_socket, "Hello World", 12);
		}


		if ( !strcmp(rBuffer, "ls") ) {

        		if ((dp = opendir(".")) == NULL) {
                		write(c_socket, err_1, strlen(err_1));
			} else {
        			while ((dir = readdir(dp)) != NULL) {
                			if (dir->d_ino == 0) continue;
		
                			write(c_socket, dir->d_name, strlen(dir->d_name));
                			write(c_socket, " ", 1);
        			}
        			closedir(dp);
			}
		}
                close(c_socket);
        }
        close(s_socket);
}
