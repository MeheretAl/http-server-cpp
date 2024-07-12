#pragma comment(lib, "ws2_32.lib")

#define PORT 8080

#include <iostream>
#include <WinSock2.h>
#include <string>

int main(int argc, char const *argv[])
{
    SOCKET wsocket;
    SOCKET new_wsocket;
    WSADATA wsaData;
    struct sockaddr_in server, client;
    int client_len;
    int BUFFER_SIZE = 30720;
    int bytes;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cerr << "Couldn't initialize Winsock" << std::endl;
        return 1;
    }

    // Create socket
    wsocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (wsocket == INVALID_SOCKET)
    {
        std::cerr << "Couldn't create socket" << std::endl;
        WSACleanup();
        return 1;
    }

    // Bind socket to address
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(PORT);

    if (bind(wsocket, (SOCKADDR *)&server, sizeof(server)) == SOCKET_ERROR)
    {
        std::cerr << "Could not bind socket" << std::endl;
        closesocket(wsocket);
        WSACleanup();
        return 1;
    }

    // Listen for incoming connections
    if (listen(wsocket, 10) == SOCKET_ERROR)
    {
        std::cerr << "Couldn't start listening" << std::endl;
        closesocket(wsocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Listening on 127.0.0.1:8080" << std::endl;

    while (true)
    {
        client_len = sizeof(client);
        new_wsocket = accept(wsocket, (SOCKADDR *)&client, &client_len);
        if (new_wsocket == INVALID_SOCKET)
        {
            std::cerr << "Could not accept connection" << std::endl;
            continue;
        }

        char buff[30720] = {0};
        bytes = recv(new_wsocket, buff, BUFFER_SIZE, 0);
        if (bytes == SOCKET_ERROR)
        {
            std::cerr << "Could not read client request" << std::endl;
            closesocket(new_wsocket);
            continue;
        }

        std::string serverMessage = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: ";
        std::string response = "<html><h1>Hello world</h1></html>";
        serverMessage.append(std::to_string(response.size()));
        serverMessage.append("\r\n\r\n");
        serverMessage.append(response);

        int totalBytesSent = 0;
        while (totalBytesSent < serverMessage.size())
        {
            int bytesSent = send(new_wsocket, serverMessage.c_str() + totalBytesSent, serverMessage.size() - totalBytesSent, 0);
            if (bytesSent == SOCKET_ERROR)
            {
                std::cerr << "Could not send response" << std::endl;
                break;
            }
            totalBytesSent += bytesSent;
        }

        std::cout << "Total Bytes sent: " << totalBytesSent << std::endl;
        closesocket(new_wsocket);
    }

    closesocket(wsocket);
    WSACleanup();
    return 0;
}
