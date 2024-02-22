#include <iostream>
#include <netdb.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "network/TcpListener.h"
#include "network/TcpClient.h"
#include "task/periodic.h"

//#include "flatbuffers/flatbuffers.h"
#include "messages/test_msg_proxy.h"

using namespace std;
using namespace std::chrono_literals;

class TestPeriodic : public task::Periodic {
public:
    explicit TestPeriodic(int period_ms) : task::Periodic(period_ms) {}

protected:
    virtual void job() override {
        cout << "Run job" << endl;
    }
};

int main() {
    cout << "Running" << endl;
    TestPeriodic p(100);
    this_thread::sleep_for(4500ms);

    cout << "Done" << endl;
    return 0;
}

int amain() {
    cout << "Test Echo server" << endl << endl;

    network::TcpListener svr(5151);
    svr.set_msg_handler([] (const char* buff, uint16_t len) {
        for(uint32_t i = 0; i < len; i++) {
            auto ch = (int)buff[i];
            cout << ch << ',';
        }
        cout << endl;
    });

    {
        network::TcpClient client("127.0.0.1", "5151");
        client.send("abc", 3);
    }

    svr.wait();

    cout << "Done" << endl;
    return 0;
}

int bmain() {
    const char* addr = "127.0.0.1";
    addrinfo hint {0}, *res;
    hint.ai_family = AF_INET;
    hint.ai_socktype = SOCK_STREAM;
    getaddrinfo(addr, "5151", &hint, &res);

    cout << "Test Echo server" << endl << endl;

    network::TcpListener svr(5151);
    svr.set_msg_handler([] (const char* buff, uint16_t len) {
        for(uint32_t i = 0; i < len; i++) {
            auto ch = (int)buff[i];
            cout << ch << ',';
        }
        cout << endl;
    });

    auto sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    connect(sockfd, res->ai_addr, res->ai_addrlen);
    auto buff = "abc";
    send(sockfd, buff, 4, 0);
    close(sockfd);

    svr.wait();

    cout << "Done" << endl;
    return 0;
}
