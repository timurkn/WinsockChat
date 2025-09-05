#pragma once
#include "Socket.h"
#include <string>

class Client {
private:
    Socket sock;
    string name;
    bool running;

public:
    Client(const string &name);
    ~Client();

    void start();
    void connectToServer(const string &host, int port);
    void sendMessage(string &message);
    void startReceiving();
};


