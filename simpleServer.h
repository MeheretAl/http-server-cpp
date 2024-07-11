#ifndef SERVER_H
#define SERVER_H

#include <vector>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

using namespace std;

class simpleServer
{
public:
    simpleServer();
    ~simpleServer();

    void init(int port);

    void bindSocket();

    void listenForConnections();

    void acceptConnection();

    void handleClient(int clientSocket);

    void closeServer();

    void handleErrors(string &msg);

private:
    int serverSocket;

    struct sockaddr_in serverAddr;

    vector<int> clientSockets;
};

#endif