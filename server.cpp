#include "server.h"
#include <iostream>

using namespace std;

server::server()
{

    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    /*
    the code above creates a socket and returns a file descriptor(an integer)
    AF_INET means Address Family Internet and is used for IPV4 addresses
    SOCK_STREAM is the type of socket we're creating , the alternative is
    SOCK_DGRAM which is used for connectionless communication,
    the third parameter 0 uses the default protocol for the type of
    socket createdm, in this case it is TCP.
    */
    if (serverSocket < 0)
    {
        handleError("Error creating socket");
    }
};

server::~server()
{
    closeServer();
}

void server::init(int port)
{
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    /*
    this function intializes the serverAddr struct which is used to hold the address information
    for the server socket
    1) sin_family -> specifies the address family, AF_INET = IPv4 address
    2) sin_addr -> a nested struct which is used to specifically deal with IP address
      2.1) s_addr -> holds the actual IP address in network-communication suitable format,
                    32-bit IPv4 address
    3) sin_port -> specifies the port number through which a socket communicates
                htons() / host-to-network short / converts the byte order of the port number
    */
}

void server::bindSocket()
{
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr) < 0))
    {
        handleError("Error binding socket");
    }

    /*
       this function binds the server socket to a specific IP address
       and port specified in the serverAddr struct
       bind() takes 3 arguments
        1) socket descriptor ( serverSocket)
        2) a pointer to struct sockaddr, so serverAddr must be cast into
            a pointer of struct sockAddr for compatibility
        3) the size of the sockaddr struct ,
            sizeof() specifies length of the address struct
    */
}
