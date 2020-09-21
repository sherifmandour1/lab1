#include "tcp_client.h"
#include <stdbool.h>
#define SPACE_CHAR " "


// This function is responsible for printing the usage message to the client if the format was wrong
void printUsage()
{
    printf("Usage: tcp_client [--help] [-v] [-h HOST] [-p PORT] ACTION MESSAGE\n\n");
    printf("Arguments:\n");
    printf("  ACTION     Must be uppercase, lowercaser, title-case,\n reverse, or shuffle.\n");
    printf("  MESSAGE    Message tp send to the server\n\n");
    printf("Options:\n  --help\n  -v, -verbose\n  --host HOSTNAME, -h HOSTNAME\n  --port PORT, -PORT");
}
// Parses the options given to the program. It will return a Config struct with the necessary
// information filled in. argc and argv are provided by main. If an error occurs in processing the
// arguments and options (such as an invalid option), this function will print the correct message
// and then exit.
Config tcp_client_parse_arguments(int argc, char *argv[])
{
    Config myConfig;
    myConfig.action = "";
    myConfig.message ="";
    myConfig.port = "";
    myConfig.host = "";
    int selectedOption = 0;
    int optionIndex = 0;
    bool portReceived = false;
    bool hostReceived = false;
    bool verboseFlag = false;
    log_set_quiet(true);
    char print[1024] = "";
    struct option long_opts[] =
    {
        {"help", no_argument, 0, 0},
        {"verbose", no_argument, 0, 'v'},
        {"host", required_argument, 0, 'h'},
        {"port", required_argument, 0, 'p'},
        {0,0,0,0}
    };

    while((selectedOption = getopt_long(argc, argv, "vh:p:", long_opts, &optionIndex)) != TCP_CLIENT_BAD_SOCKET)
    {
        switch(selectedOption)
        {
            case 'v':
            verboseFlag = true;
            log_set_quiet(false);
            log_trace("About to parse terminal inputs..");
            break;
            case 'h':
            hostReceived = true;
            log_info("Received a host from client and the value is %s", optarg);
            myConfig.host = optarg;
            break;
            case 'p':
            log_info("Received port from the client and the value is %d" ,myConfig.port);
            portReceived = true;
            myConfig.port = optarg;
            sprintf(print, "this is the port %s", optarg);
            break;
            case '?':
            break;
            default:
            log_error("Unkonwn argument provided\n");
            printUsage();
            break;
        }
    }
    argc -= optind;
    argv += optind;
    if(!portReceived)
    {
        myConfig.port = TCP_CLIENT_DEFAULT_PORT;
    }
    if(!hostReceived)
    {
        myConfig.port = TCP_CLIENT_DEFAULT_PORT;
    }
    if(argc == 0)
    {
        printUsage();
    }
    else if(argc == 1)
    {
        log_error("Missing action or message, Please review format and try again");
        printUsage();
        exit(EXIT_SUCCESS);
    }
    else if(argc == 2)
    {
        myConfig.action = argv[0];
        myConfig.message = argv[1];
    }
    else
    {
        printUsage();
        exit(EXIT_SUCCESS);
    }
    
    log_info("Action and message were received and the values are Action: %s, Message: %s\n", myConfig.action, myConfig.message);
    if(strcmp(myConfig.action, "reverse") == 0 || strcmp(myConfig.action, "uppercase") == 0 || strcmp(myConfig.action, "lowercase") == 0 || strcmp(myConfig.action, "shuffle") == 0 || strcmp(myConfig.action, "title-case") == 0 )
    {
        log_info("Action is valid and matches permitted arguments");
    }
    else
    {
        printUsage();
        exit(EXIT_SUCCESS);
        log_error("\"%s\" is an invalid action", myConfig.action);
    }
    
        return myConfig;
        
}


////////////////////////////////////////////////////
///////////// SOCKET RELATED FUNCTIONS /////////////
////////////////////////////////////////////////////

// Creates a TCP socket and connects it to the specified host and port. It
// returns the socket file descriptor.
// this function takes in the struct config which should already be loaded with the action, message , host, and port
int tcp_client_connect(Config config)
{
    log_trace("Attempting the connection to the sever");
    int serverSocket;
    struct hostent *he;
    struct sockaddr_in serverAddr;
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSocket == -1)
    {
        log_error("Socket creation was terminated");
        exit(EXIT_SUCCESS);
    }
    else
    {
        log_info("Socket created succesfully");
    }
      bzero(&serverAddr, sizeof(serverAddr));
      if ( (he = gethostbyname(config.host) ) == NULL ) 
      {
      exit(EXIT_SUCCESS);
      }

    /* copy the network address to sockaddr_in structure */
    memcpy(&serverAddr.sin_addr, he->h_addr_list[0], he->h_length);
    serverAddr.sin_port = htons(atoi(config.port)); //FIX ME


    
    if(connect(serverSocket, (struct sockaddr*) &serverAddr, sizeof(serverAddr)) != 0)
    {
        log_trace("Establishing connection was not complete");
        exit(EXIT_SUCCESS);
    }
    else
    {
        log_info("Successfully connected to the server");
    }
    return serverSocket;
}

// Using the the action and message provided by the command line, format the
// data to follow the protocol and send it to the server.
// this function takes in the socketnumber, the action, and the message that are stores in the struct to send it to the server
void tcp_client_send_message(int sockfd, char *action, char *message)
{
    char messageLength[10];
    char messageFormat[1024] = "";
    strcpy(messageFormat, action);
    strncat(messageFormat, SPACE_CHAR, 1);
    sprintf(messageLength,"%lu", strlen(message));
    strncat(messageFormat, messageLength, (strlen(messageLength)) );
    strncat(messageFormat, SPACE_CHAR, 1);
    strncat(messageFormat, message, 1024);
    write(sockfd, &messageFormat, sizeof(messageFormat));
    log_info("Message sent");
}

// Receive the response from the server. The caller must provide a buffer and
// the size of the buffer provided.
//this function takes in the socket number, a buffer that should be provided by the caller and a size of the buffer so it can be populated by the message received
void tcp_client_receive_response(int sockfd, char *buf, int buf_size)
{
    log_trace("Currently receiving response from server");
    read(sockfd, buf, buf_size);
    log_info("Successfully received the info from the server, and the response is:");
}

// Close the socket when your program is done running.
void tcp_client_close(int sockfd)
{
    log_trace("Attempting to close the server socket..");
    close(sockfd);
    log_info("Socket successfully closed");
}