#include "Socket.h"
#include <iostream>

Socket::Socket() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
}

Socket::~Socket() {
    close();
    WSACleanup();
}

bool Socket::create() {
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    return sock != INVALID_SOCKET;
}

bool Socket::bind(int port) {
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    return ::bind(sock, (sockaddr*)&addr, sizeof(addr)) != SOCKET_ERROR;
}

bool Socket::listen(int backlog) {
    return ::listen(sock, backlog) != SOCKET_ERROR;
}

bool Socket::connect(const string &host, int port) {
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, host.c_str(), &addr.sin_addr);

    if (::connect(sock, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
        return false;

    connected = true;
    return true;
}

unique_ptr<Socket> Socket::accept() {
    sockaddr_in clientAddr;
    int addrLen = sizeof(clientAddr);

    SOCKET clientSock = ::accept(sock, (sockaddr*)&clientAddr, &addrLen);
    if (clientSock == INVALID_SOCKET) return nullptr;

    auto client = make_unique<Socket>();
    client->sock = clientSock;
    client->connected = true;
    return client;
}

int Socket::send(const string &data) {
    return ::send(sock, data.c_str(), data.size(), 0);
}

int Socket::recv(string &data) {
    char buffer[1024];
    int bytes = ::recv(sock, buffer, sizeof(buffer), 0);
    if (bytes > 0)
        data.assign(buffer, bytes);
    return bytes;
}

void Socket::close() {
    if (sock != INVALID_SOCKET) {
        closesocket(sock);
        sock = INVALID_SOCKET;
    }
    connected = false;
}

bool Socket::isConnected() {
    return connected;
}

SOCKET Socket::getSocket() {
    return sock;
}
