#include "tcp_client.h"
#include <stdio.h>
#include <stdlib.h>



int main(int argc, char *argv[])
{
    int mainSockFd = 0;
    char response[1024];
    bzero(response,sizeof(response));
    Config bro = tcp_client_parse_arguments(argc, argv);
    mainSockFd = tcp_client_connect(bro);
    tcp_client_send_message(mainSockFd, bro.action, bro.message);
    tcp_client_receive_response(mainSockFd, response, sizeof(response));
    printf("%s\n", response);

    return 0;
}
