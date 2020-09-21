#ifndef TCP_CLIENT_H_
#define TCP_CLIENT_H_

#include <arpa/inet.h>
#include <errno.h>
#include <getopt.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define TCP_CLIENT_BAD_SOCKET -1
#define TCP_CLIENT_DEFAULT_PORT "8080"
#define TCP_CLIENT_DEFAULT_HOST "localhost"
#define TCP_CLIENT_MAX_RECEIVE_SIZE 1024

// Contains all of the information needed to create to connect to the server and
// send it a message.
typedef struct Config {
    char *port;
    char *host;
    char *action;
    char *message;
} Config;

// Parses the options given to the program. It will return a Config struct with the necessary
// information filled in. argc and argv are provided by main. If an error occurs in processing the
// arguments and options (such as an invalid option), this function will print the correct message
// and then exit.
Config tcp_client_parse_arguments(int argc, char *argv[]);

////////////////////////////////////////////////////
///////////// SOCKET RELATED FUNCTIONS /////////////
////////////////////////////////////////////////////

// Creates a TCP socket and connects it to the specified host and port. It
// returns the socket file descriptor.
int tcp_client_connect(Config config);

// Using the the action and message provided by the command line, format the
// data to follow the protocol and send it to the server.
void tcp_client_send_message(int sockfd, char *action, char *message);

// Receive the response from the server. The caller must provide a buffer and
// the size of the buffer provided.
void tcp_client_receive_response(int sockfd, char *buf, int buf_size);

// Close the socket when your program is done running.
void tcp_client_close(int sockfd);

#endif
