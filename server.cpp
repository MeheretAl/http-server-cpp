#include "server.h"
#include <iostream>
#include <string.h>

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

void server::listenForConnections()
{
    if (listen(serverSocket, 5) < 0)
    {
        handleError("Error listening for connections");
    }

    /*
    this function prepares the socket to accept incoming client connections

    1) serverSocket is the socket descriptor bound to a specific address
    2) 5 is the maximum length of the queue of pending connections
       that the operating system should allow for this socket.
    */
}

void server::acceptConnection()
{
    struct sockaddr_in clientAddr;
    socklen_t clientLen = sizeof(clientAddr);
    int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, sizeof(clientAddr) < 0);
    if (clientSocket < 0)
    {
        handleError("Error accepting connection");
    }
    else
    {
        handleClient(clientSocket);
    }

    /*
   This function accepts incoming client connections on the server socket.

   1) `serverSocket` is the socket descriptor bound to a specific address and port.
   2) `clientAddr` is a structure that will hold the client's address information after connection.
   3) `clientLen` is the size of the `clientAddr` structure.
   4) `accept()` returns a new socket descriptor (`clientSocket`) for the accepted connection.
      - It waits until a client connects and then returns the socket descriptor for that client.
      - The `clientAddr` parameter is filled with the client's address information.
   5) If `accept()` fails (returns `-1`), it calls `handleError()` with an appropriate error message.
   6) If successful, it calls `handleClient()` to handle communication with the accepted client.
*/
}

void server::handleClient(int clientSocket)
{
    char buffer[256];
    int n = recv(clientSocket, buffer, 255, 0);
    if (n < 0)
    {
        handleError("Error reading from socket");
    }

    buffer[n] = '\0';

    cout << "Client message: " << buffer << endl;

    n = send(clientSocket, "Message received", 16, 0);

    if (n < 0)
    {
        handleError("Error writing to socket");
    }

    closesocket(clientSocket);

    /*
   This function handles communication with a connected client.

   1) `clientSocket` is the socket descriptor for the client connection.
   2) `buffer` is an array to store incoming data from the client.
   3) `recv()` reads data from `clientSocket` into `buffer`.
      - It reads up to 255 bytes of data.
      - Returns the number of bytes received, or `-1` on error.
   4) If `recv()` fails (returns `-1`), calls `handleError()` with an error message.
   5) Prints the client's message to the console.
   6) `send()` sends a response (`"Message received"`) back to the client via `clientSocket`.
      - Returns the number of bytes sent, or `-1` on error.
   7) If `send()` fails (returns `-1`), calls `handleError()` with an error message.
   8) `closesocket()` closes `clientSocket` to release resources.
    */
}

void server::closeServer()
{

    closesocket(serverSocket);

    for (int clientSocket : clientSockets)
    {
        closesocket(clientSocket);
    }

    /*
   Close the server and all connected client sockets.

   - `serverSocket`: Socket descriptor for the server.
     - Closed using `closesocket()`.

   - `clientSockets`: List of socket descriptors for connected clients.
     - Each client socket is closed using `closesocket()` in a loop.
*/
}

void server::handleError(const std::string &msg)
{
    perror(msg.c_str());
    exit(1);

    /*
   This function handles errors by printing an error message and terminating the program.

   1) `msg` is a reference to a `std::string` containing the error message.
   2) `perror()` prints a descriptive error message to `stderr` (standard error output).
      - Converts `msg` to a C-style string (`const char*`) using `c_str()`.
   3) `exit(1)` terminates the program with a non-zero status code indicating an error.
*/
}