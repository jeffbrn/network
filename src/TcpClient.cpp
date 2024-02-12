#include "TcpClient.h"

#include <netdb.h>
#include <stdexcept>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>

using namespace  std;

namespace network {

    TcpClient::TcpClient(const char *addr, const char *port) {
        addrinfo hint {0}, *res;
        hint.ai_family = AF_INET;
        hint.ai_socktype = SOCK_STREAM;
        auto rv = getaddrinfo(addr, port, &hint, &res);
        if (rv != 0) throw runtime_error("Invalid address and port");

        cout << "Client: connecting to: " << addr << ':' << port << endl;
        skt_fd_ = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if (skt_fd_ <= 0) throw runtime_error("Unable to create socket");
        struct timeval tv { 1, 0 };
        setsockopt(skt_fd_, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);

        rv = connect(skt_fd_, res->ai_addr, res->ai_addrlen);
        freeaddrinfo(res);
        if (rv != 0) throw runtime_error("Unable to connect");
        cout << "client: connected" << endl;
    }

    TcpClient::~TcpClient() {
        close(skt_fd_);
        cout << "TcpClient destructor" << endl;
    }

    void TcpClient::send(const char *, uint16_t) {
        auto tmp = "abc";
        cout << "client: sending message" << endl;
        auto rv = ::send(skt_fd_, tmp, 4, 0);
        cout << "Client: send rv value = " << rv << endl;
        ssize_t bytes_read;
        cout << "client: receiving response" << endl;
        while ((bytes_read = recv(skt_fd_, buff_, BUFF_SZ, 0)) > 0) {
            cout << "Client: Recvd: " << bytes_read << " bytes: ";
            for(int i = 0; i < bytes_read; i++)
                cout << (int)buff_[i] << ',';
            cout << endl;
        }
    }

}
