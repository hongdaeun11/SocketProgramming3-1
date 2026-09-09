#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <pthread.h>
#include <signal.h>

#define CHATDATA 1024

void* do_send_chat(void *);
void* do_receive_chat(void *);

pthread_t thread_1, thread_2;
char escape[] = "exit";
char nickname[20];

int main(int argc, char *argv[])
{
    int c_socket;
    struct sockaddr_in c_addr;

    if (argc < 3) {
        printf("usage : %s ip_address port_number\n", argv[0]);
        exit(-1);
    }

    c_socket = socket(PF_INET, SOCK_STREAM, 0);

    memset(&c_addr, 0, sizeof(c_addr));
    c_addr.sin_addr.s_addr = inet_addr(argv[1]);
    c_addr.sin_family = AF_INET;
    c_addr.sin_port = htons(atoi(argv[2]));

    printf("Input Nickname : ");
    scanf("%s", nickname);

    if (connect(c_socket, (struct sockaddr *)&c_addr, sizeof(c_addr)) == -1) {
        printf("Can not connect\n");
        return -1;
    }

    write(c_socket, nickname, strlen(nickname));

    pthread_create(&thread_1, NULL, do_send_chat, (void *)(long)c_socket);
    pthread_create(&thread_2, NULL, do_receive_chat, (void *)(long)c_socket);

    pthread_join(thread_1, NULL);
    pthread_join(thread_2, NULL);

    close(c_socket);
    return 0;
}

void *do_send_chat(void* arg)
{
    char chatData[CHATDATA];
    char buf[CHATDATA];
    int n;
    int c_socket = (int)(long)arg;

    while(1) {
        memset(buf, 0, sizeof(buf));
        if ((n = read(0, buf, sizeof(buf))) > 0) {
            buf[n] = '\0';
            if (!strncmp(buf, escape, strlen(escape))) {
                write(c_socket, buf, n);
                pthread_kill(thread_2, SIGINT);
                break;
            }
            
            write(c_socket, buf, strlen(buf));
        }
    }
    return NULL;
}

void *do_receive_chat(void* arg)
{
    char chatData[CHATDATA];
    int n;
    int c_socket = (int)(long)arg;

    while(1) {
        memset(chatData, 0, sizeof(chatData));
        if ((n = read(c_socket, chatData, sizeof(chatData))) > 0) {
            write(1, chatData, n);
        }
    }
    return NULL;
}
