#include "Client.h"
#include <iostream>
#include <thread>

Client::Client(const string &name) : name(name), running(false) {}

Client::~Client() {
    running = false;
    sock.close();
}

void Client::connectToServer(const string &host, int port) {
    if (!sock.connect(host, port)) {
        cout << "Connection failed!" << endl;
        exit(1);
    }
    sendMessage(name);
    cout << "You have joined the chat" << endl;
    running = true;
}

void Client::sendMessage(string &message) {
    if (sock.send(message) < 0) {
        cout << "Send failed!" << endl;
    }
}

void Client::startReceiving() {
    thread([&]() {
       while (running) {
           string message;
           int bytes = sock.recv(message);
           if (bytes <= 0) {
               cout << "Disconnected" << endl;
               break;
           }
           cout << message << endl;
       }
    }).detach();
}

void Client::start()  {
    if (!sock.create()) {
        cout << "Create socket failed" << endl;
        exit(1);
    }

    connectToServer("127.0.0.1", 8080);
    startReceiving();

    string userMessage;
    while (true) {
        getline(cin, userMessage);
        if (userMessage.empty()) continue;
        if (userMessage == "/quit") break;
        sendMessage(userMessage);
    }
}
