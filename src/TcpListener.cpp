#include "TcpListener.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdexcept>

using namespace std;

namespace network {

    TcpListener::TcpListener(int portnum) {
        sockaddr_in svr_addr {};
        svr_addr.sin_family = AF_INET;
        svr_addr.sin_addr.s_addr = INADDR_ANY;
        svr_addr.sin_port = htons(portnum);
        skt_fd_ = socket(AF_INET, SOCK_STREAM, 0);
        if (skt_fd_ == -1) throw runtime_error("Failed to create socket");

        if (bind(skt_fd_, reinterpret_cast<const sockaddr *>(&svr_addr), sizeof(svr_addr)) < 0)
            throw runtime_error("Failed to bind socket");

        thrd_ = std::move(thread {&TcpListener::handler, this });
    }

    TcpListener::~TcpListener() {
        if (thrd_.joinable()) {
            thrd_.join();
        }
        close(skt_fd_);
    }

    void TcpListener::handler() {
        if (skt_fd_ == -1) return;
    }

}
