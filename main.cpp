#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

const int BUFFER_SIZE = 1024;
const int PORT = 5151;

using namespace std;

int main() {
    cout << "Hello world!" << endl;
    int serverSocket, clientSocket;
    sockaddr_in serverAddress {}, clientAddress {};
    uint8_t buffer[BUFFER_SIZE];

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) throw runtime_error("Failed to create socket");

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(PORT);

    if (bind(serverSocket, reinterpret_cast<const sockaddr *>(&serverAddress), sizeof(serverAddress)) < 0) {
        throw runtime_error("Failed to bind socket");
    }
    listen(serverSocket, 0);
    cout << "Server listening on port " << PORT << endl;

    socklen_t clientAddressLength = sizeof(clientAddress);
    clientSocket = accept(serverSocket, reinterpret_cast<sockaddr *>(&clientAddress), &clientAddressLength);
    if (clientSocket < 0) {
        throw runtime_error("Failed to accept client connection");
    }
    cout << "Client connected: " << inet_ntoa(clientAddress.sin_addr) << endl;

    ssize_t byesRead;
    while ((byesRead = recv(clientSocket, buffer, BUFFER_SIZE, 0)) > 0) {
        send(clientSocket, buffer, byesRead,0);
        cout << "Recvd and sent: " << byesRead << " bytes" << endl;

        if (byesRead == 0) {
            cout << "Client disconnected" << endl;
        } else if (byesRead < 0) {
            throw runtime_error("error receiving data");
        }
    }

    close(clientSocket);
    close(serverSocket);

    return 0;
}