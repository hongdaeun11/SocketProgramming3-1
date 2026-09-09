// sock_opt.c
#include        <sys/socket.h>
#include        <stdio.h>

int
main()
{
        int     sock;
        int     val, len;

        struct  sock_opts {
                const char      *opt_str;
                int             opt_level;
                int             opt_name;
        } sock_opts[] = {
                { "SO_BROADCAST",       SOL_SOCKET, SO_BROADCAST },
                { "SO_KEEPALIVE",        SOL_SOCKET, SO_KEEPALIVE },
                { "SO_LINGER",            SOL_SOCKET, SO_LINGER },
                { "SO_REUSEADDR",       SOL_SOCKET, SO_REUSEADDR },
                { "SO_DONTROUTE",      SOL_SOCKET, SO_DONTROUTE },
                { "SO_RCVBUF",           SOL_SOCKET, SO_RCVBUF },
                { "SO_SNDBUF",           SOL_SOCKET, SO_SNDBUF },
                { "SO_TYPE",              SOL_SOCKET, SO_TYPE },
                { NULL,                 0,          0 }
        };

        struct  sock_opts       *ptr;

        sock = socket(AF_INET, SOCK_STREAM, 0);

        for (ptr = sock_opts; ptr->opt_str != NULL; ptr++) {

                len = sizeof(val);
                getsockopt(sock, ptr->opt_level, ptr->opt_name, (void *)&val, &len);


                printf("%s :\t%d\n", ptr->opt_str, val);
        }
}
