#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>

#define MAX_CLIENT 10
#define MAX_NAME 20
#define CHATDATA 1024
#define INVALID_SOCK -1

struct user_info {
    int usersocket;
    char username[MAX_NAME];
};

struct user_info user_list[MAX_CLIENT];

void* do_chat(void *);
int pushClient(int c_socket, char *nickname);
int popClient(int s);

pthread_t thread;
pthread_mutex_t mutex;

char escape[] = "exit";
char greeting[] = "Welcome to chatting room\n";
char CODE200[] = "Sorry No More Connection\n";

int main(int argc, char *argv[]) {
    int c_socket, s_socket;
    struct sockaddr_in s_addr, c_addr;
    int len, i, n;
    char nickname[MAX_NAME];

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

    if (listen(s_socket, MAX_CLIENT) == -1) {
        printf("listen Fail\n");
        return -1;
    }

    for (i = 0; i < MAX_CLIENT; i++)
        user_list[i].usersocket = INVALID_SOCK;

    while (1) {
        len = sizeof(c_addr);
        c_socket = accept(s_socket, (struct sockaddr *)&c_addr, &len);

        memset(nickname, 0, sizeof(nickname));
        n = read(c_socket, nickname, sizeof(nickname));
        if (n > 0) {
            nickname[n] = '\0';
        }

        if (pushClient(c_socket, nickname) < 0) {
            write(c_socket, CODE200, strlen(CODE200));
            close(c_socket);
        } else {
            write(c_socket, greeting, strlen(greeting));
            pthread_create(&thread, NULL, do_chat, (void *)(long)c_socket);
        }
    }
}

void *do_chat(void *arg) {
    int c_socket = (int)(long)arg;
    char chatData[CHATDATA];
    int i, n;
    char my_name[MAX_NAME];

    pthread_mutex_lock(&mutex);
    for (i = 0; i < MAX_CLIENT; i++) {
        if (user_list[i].usersocket == c_socket) {
            strcpy(my_name, user_list[i].username);
            break;
        }
    }
    pthread_mutex_unlock(&mutex);

    while (1) {
        memset(chatData, 0, sizeof(chatData));
        if ((n = read(c_socket, chatData, sizeof(chatData))) > 0) {
            
            if (chatData[0] == '/') {
                char copy[CHATDATA];
                strcpy(copy, chatData);
                char *target_name = strtok(copy + 1, " ");
                char *msg = strtok(NULL, "");

                if (target_name && msg) {
                    int found = 0;
                    pthread_mutex_lock(&mutex);
                    for (i = 0; i < MAX_CLIENT; i++) {
                        if (user_list[i].usersocket != INVALID_SOCK && strcmp(user_list[i].username, target_name) == 0) {
                            char whisper[CHATDATA + MAX_NAME];
                            sprintf(whisper, "[귓속말]%s: %s", my_name, msg);
                            write(user_list[i].usersocket, whisper, strlen(whisper));
                            found = 1;
                            break;
                        }
                    }
                    pthread_mutex_unlock(&mutex);
                    if (found) continue;
                }
            }

            char formattedData[CHATDATA + MAX_NAME];
            sprintf(formattedData, "[%s] %s", my_name, chatData);

            pthread_mutex_lock(&mutex);
            for (i = 0; i < MAX_CLIENT; i++) {
                if (user_list[i].usersocket != INVALID_SOCK) {
                    write(user_list[i].usersocket, formattedData, strlen(formattedData));
                }
            }
            pthread_mutex_unlock(&mutex);

            if (strstr(chatData, escape) != NULL) {
                popClient(c_socket);
                break;
            }
        } else {
            popClient(c_socket);
            break;
        }
    }
    return NULL;
}

int pushClient(int c_socket, char *nickname) {
    int i;
    for (i = 0; i < MAX_CLIENT; i++) {
        pthread_mutex_lock(&mutex);
        if (user_list[i].usersocket == INVALID_SOCK) {
            user_list[i].usersocket = c_socket;
            strcpy(user_list[i].username, nickname);
            pthread_mutex_unlock(&mutex);
            return i;
        }
        pthread_mutex_unlock(&mutex);
    }
    if(i == MAX_CLIENT) return -1;

}

int popClient(int s) {
    int i;
    close(s);
    for (i = 0; i < MAX_CLIENT; i++) {
        pthread_mutex_lock(&mutex);
        if (s == user_list[i].usersocket) {
            user_list[i].usersocket = INVALID_SOCK;
            memset(user_list[i].username, 0, MAX_NAME);
            pthread_mutex_unlock(&mutex);
            break;
        }
        pthread_mutex_unlock(&mutex);
    }
    return 0;
}
