#pragma once
#include "Socket.h"
#include <vector>
#include <string>
#include <memory>

class Server {
private:
    Socket sock;
    bool running;
    vector<unique_ptr<Socket>> clientSockets;

public:
    Server();
    ~Server();

    void sendAllClients(const string &message);
    void startReceiving();
    void start();
    void removeDisconnectedClients();
};