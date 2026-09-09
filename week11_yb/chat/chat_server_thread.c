// chat_server_thread.c
#include        <stdio.h>
#include        <stdlib.h>
#include        <string.h>
#include        <unistd.h>
#include        <netinet/in.h>
#include        <sys/socket.h>
#include	<pthread.h>

void* 		do_chat(void *);

pthread_t	thread;
pthread_mutex_t	mutex;

typedef struct {
	int socket;
	char nickname[20];

} ChatClient;


#define         MAX_CLIENT      10
#define         CHATDATA        1024

#define         INVALID_SOCK        -1

ChatClient     	list_c[MAX_CLIENT];

char    	escape[] = "exit";
char    	greeting[] = "Welcome to chatting room\n";
char    	CODE200[] = "Sorry No More Connection\n";

main(int argc, char *argv[])
{
        int     c_socket, s_socket;
        struct  sockaddr_in s_addr, c_addr;
        int     len;
        int     nfds = 0;
        int     i, j, n;
        fd_set  read_fds;
	int	res;

        if (argc < 2) {
                printf("usage: %s port_number\n", argv[0]);
                exit(-1);
        }

	if (pthread_mutex_init(&mutex, NULL) != 0) {
		printf("Can not create mutex\n");
		return -1;
	}

        s_socket = socket(PF_INET, SOCK_STREAM, 0);

        memset(&s_addr, 0, sizeof(s_addr));
        s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        s_addr.sin_family = AF_INET;
        s_addr.sin_port = htons(atoi(argv[1]));

        if (bind(s_socket, (struct sockaddr *)&s_addr, sizeof(s_addr)) == -1) {
                printf("Can not Bind\n");
                return -1;
        }

        if(listen(s_socket, MAX_CLIENT) == -1) {
                printf("listen Fail\n");
                return -1;
        }

	for (i = 0; i < MAX_CLIENT; i++)
		list_c[i].socket = INVALID_SOCK;

	while(1) {
		len = sizeof(c_addr);
		c_socket = accept(s_socket, (struct sockaddr *)&c_addr, &len);

		res = pushClient(c_socket);
		if (res < 0) {
			write(c_socket, CODE200, strlen(CODE200));
			close(c_socket);
		} else {
			write(c_socket, greeting, strlen(greeting));
			pthread_create(&thread, NULL, do_chat, (void *) c_socket);
		}	
	}
}

void *
do_chat(void *arg) 
{
	int c_socket = (int) arg;
        char    chatData[CHATDATA];
	int	i, n;

	char my_nick[20];
	memset(my_nick, 0, sizeof(my_nick));
	read(c_socket, my_nick, sizeof(my_nick));

	pthread_mutex_lock(&mutex);
	for(i=0; i<MAX_CLIENT; i++){
		if(list_c[i].socket == c_socket){
			strcpy(list_c[i].nickname, my_nick);
			break;
		}
	}
	pthread_mutex_unlock(&mutex);

	while(1) {
		memset(chatData, 0, sizeof(chatData));
          	if ((n = read(c_socket, chatData, sizeof(chatData))) > 0 ) {

			if(chatData[0] == '/'){
				char *target_nick = strtok(chatData + 1, " ");
				char *msg = strtok(NULL, "");
				
				if(target_nick && msg){
					int found = 0;
					for(i = 0; i < MAX_CLIENT; i++){
						pthread_mutex_lock(&mutex);
						if(list_c[i].socket != INVALID_SOCK && strcmp(list_c[i].nickname, target_nick) == 0){
							char whisper[CHATDATA];
							sprintf(whisper, "[귓속말] %s: %s", my_nick,msg);
							write(list_c[i].socket, whisper, strlen(whisper));
							pthread_mutex_unlock(&mutex);
							found = 1;
							break;
						}
						pthread_mutex_unlock(&mutex);
					}
					if(found) continue;
				}
			}
			
                	for (i = 0; i < MAX_CLIENT; i++) {
				pthread_mutex_lock(&mutex);
                        	if (list_c[i].socket != INVALID_SOCK) {
                                	write(list_c[i].socket, chatData, n);
				}
				pthread_mutex_unlock(&mutex);
			}

                        if(strstr(chatData, escape) != NULL) {
                      		popClient(c_socket);
				break;
                        }
            	}else{
			popClient(c_socket);
			break;
		}

        }
	return NULL;
}

int
pushClient(int c_socket) {
	int	i;

	for (i = 0; i < MAX_CLIENT; i++) {
		pthread_mutex_lock(&mutex);
		if(list_c[i].socket == INVALID_SOCK) {
			list_c[i].socket = c_socket;
			pthread_mutex_unlock(&mutex);
			return i;
		}	
		pthread_mutex_unlock(&mutex);
	}

	if (i == MAX_CLIENT)
		return -1;
}

int
popClient(int s)
{
        int     i;

        close(s);

        for (i = 0; i < MAX_CLIENT; i++) {
		pthread_mutex_lock(&mutex);
                if ( s == list_c[i].socket ) {
			list_c[i].socket = INVALID_SOCK;
			pthread_mutex_unlock(&mutex);
			break;
		}
		pthread_mutex_unlock(&mutex);
        }

        return 0;
}
