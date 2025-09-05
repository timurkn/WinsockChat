#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <memory>

using namespace std;

class Socket {
private:
    SOCKET sock = INVALID_SOCKET;
    bool connected = false;

public:
    Socket();
    ~Socket();

    bool create();
    bool bind(int port);
    bool listen(int backlog = 10);
    bool connect(const string &host, int port);
    unique_ptr<Socket> accept();

    int send(const string &data);
    int recv(string &data);

    void close();
    bool isConnected();
    SOCKET getSocket();
};
