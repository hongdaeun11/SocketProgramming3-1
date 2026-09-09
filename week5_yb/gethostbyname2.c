// gethostbyname.c
#include	<stdio.h>
#include        <netdb.h>
#include 	<stddef.h>
#include        <sys/socket.h>

main(int argc, char* argv[]) {
        struct  in_addr addr;
        struct  hostent *host;
        const   char    *hostName = "www.yahoo.com";
        int     i;


        if ( (host = gethostbyname(argv[1])) == NULL) {
                printf("gethostbyname() error - check network\n");
                exit(-1);
        }

        printf("official name = %s\n", host->h_name);

        i = 0;
        while( host->h_aliases[i] != NULL ) {
                printf("aliases = %s\n", host->h_aliases[i++]);
        }

        printf("address type = %d\n", host->h_addrtype);
        printf("address length = %d\n", host->h_length);

        i = 0;
        while (host->h_addr_list[i] != NULL) {
                memcpy(&addr.s_addr, host->h_addr_list[i], 4);
                printf("address = (0x%x)\n", ntohl(*(long *)host->h_addr_list[i]));
                i++;
        }
}
