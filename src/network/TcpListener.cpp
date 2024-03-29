#include "network/TcpListener.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <chrono>
#include <stdexcept>
#include <iostream>
#include <utility>

using namespace std;

namespace network {

    TcpListener::TcpListener(uint16_t portnum) : port_{portnum} {
        sockaddr_in svr_addr {};
        svr_addr.sin_family = AF_INET;
        svr_addr.sin_addr.s_addr = INADDR_ANY;
        svr_addr.sin_port = htons(portnum);
        skt_fd_ = socket(AF_INET, SOCK_STREAM, 0);
        if (skt_fd_ == -1) throw runtime_error("Failed to create socket");

        if (bind(skt_fd_, reinterpret_cast<const sockaddr *>(&svr_addr), sizeof(svr_addr)) < 0)
            throw runtime_error("Failed to bind socket");

        running_.test_and_set();
        thrd_ = std::move(thread {&TcpListener::handler, this });
        this_thread::sleep_for(10ms);
    }

    TcpListener::~TcpListener() {
        wait();
        close(skt_fd_);
        cout << "TcpListener destructor" << endl;
    }

    void TcpListener::set_msg_handler(std::function<void (const char*, uint16_t)> handle) {
        msg_handler_ = std::move(handle);
    }

    void TcpListener::wait() {
        cout << "Server: waiting to shutdown" << endl;
        running_.clear();
        if (thrd_.joinable()) {
            thrd_.join();
        }
        cout << "Server: finished shutdown" << endl;
    }

    void TcpListener::handler() {
        if (skt_fd_ == -1) return;
        listen(skt_fd_, 0);

        while(running_.test_and_set()) {
            cout << "Server: listening on port " << port_ << endl;
            sockaddr_in client_addr {};
            socklen_t addr_len = sizeof (client_addr);
            auto client_skt = accept(skt_fd_, reinterpret_cast<sockaddr *>(&client_addr), &addr_len);
            if (client_skt < 0) {
                cout << "ERR: Failed to accept client connection" << endl << endl;
                continue;
            }

            cout << "Server: Client connected" << endl;
            ssize_t bytes_read;
            while ((bytes_read = recv(client_skt, buff_, BUFF_SZ, 0)) > 0) {
                send(client_skt, buff_, bytes_read,0);
                cout << "Server: Recvd and sent: " << bytes_read << " bytes" << endl;
                if (msg_handler_) msg_handler_(buff_, bytes_read);
            }
            close(client_skt);
            if (bytes_read < 0) {
                cout << "Server: ERR: problem receiving data" << endl << endl;
                break;
            }
            cout << "Server: Client disconnected" << endl << endl;
        }

        cout << "Server: shutting down" << endl;
    }

}
