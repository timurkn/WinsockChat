#include <iostream>

#include "Client.h"
#include <string>

int main() {
    string name;
    cout << "Enter your name:" << endl;
    getline(cin, name);
    Client client(name);
    client.start();
    return 0;
}