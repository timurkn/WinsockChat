#include "Server.h"
#include <iostream>
#include <thread>
#include <algorithm>

Server::Server() : running(false) {}

Server::~Server() {
    running = false;
    sock.close();
}

void Server::sendAllClients(const string &message) {
    removeDisconnectedClients();
    for (auto &clientSocket: clientSockets)
        if (clientSocket->isConnected())
            if (!clientSocket->send(message))
                cout << "Send Failed" << endl;
}

void Server::startReceiving() {
    while (running) {
        unique_ptr<Socket> clientSocket = sock.accept();
        if (clientSocket == nullptr) continue;

        string name;
        clientSocket->recv(name);

        string joinMessage = name + " joined to the chat!";
        cout << joinMessage  << endl;
        sendAllClients(joinMessage);

        clientSockets.push_back(move(clientSocket));
        Socket* currentSocket = clientSockets.back().get();

        thread([this, name, currentSocket]() {
            while (currentSocket  && currentSocket ->isConnected()) {
                string message;
                int bytes = currentSocket->recv(message);
                if (bytes <= 0) {
                    break;
                }
                message = "[" + name + "] " + message;
                sendAllClients(message);
                cout << message << endl;
            }
        }).detach();
    }
}


void Server::removeDisconnectedClients() {
    clientSockets.erase(
        remove_if(clientSockets.begin(), clientSockets.end(),
            [](const std::unique_ptr<Socket>& client) {
                return !client->isConnected();
            }),
        clientSockets.end()
    );
}

void Server::start() {
    if (!sock.create()) {
        cout << "Create socket failed" << endl;
        exit(1);
    }
    if (!sock.bind(8080)) {
        cout << "Bind failed" << endl;
        exit(1);
    }
    if (!sock.listen(SOMAXCONN)) {
        cout << "Listen failed" << endl;
        exit(1);
    }
    running = true;

    cout << "Server is listening" << endl;
    startReceiving();
    //thread(Server::startReceiving, this).detach();
}