#include <iostream>
#include <netdb.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "network/TcpListener.h"
#include "network/TcpClient.h"
#include "task/periodic.h"

#include "messages/test_msg_proxy.h"
#include "config/app_config.h"
#include <unistd.h>

using namespace std;
using namespace std::chrono_literals;
using namespace netw::config;

class TestPeriodic : public task::Periodic {
public:
    explicit TestPeriodic(int period_ms) : task::Periodic(period_ms) {}

protected:
    virtual void job() override {
        cout << "Run job" << endl;
    }
};

int main() {
    auto a = AppConfig::read_config();
    cout << a << endl;
    return 0;

    char cwd[256];
    if (getcwd(cwd, sizeof(cwd))) {
        cout << "Current working dir: " << cwd << endl;
    }

    AppConfig config;
    YAML::Emitter out;
    out.SetOutputCharset(YAML::EscapeNonAscii);
    out.SetIndent(4);
    out << config;
    cout << out.c_str() << endl;
/*
    netw::messages::TstMsgProxy msg {
        2,
        {11,22,33},
        "Test 2",
        true,
        {'a', 'b', 'c'}
    };
    cout << "Serialized object:" << endl;
    cout << msg << endl;

    flatbuffers::FlatBufferBuilder builder(1024);
    netw::messages::TstMsgProxy::Serialize(builder, msg);
    auto buf = builder.GetBufferPointer();
    auto size = builder.GetSize();
    auto r = netw::messages::TstMsgProxy::Deserialize(buf);

    cout << endl << "Deserialized object:" << endl;
    cout << r << endl;
    return 0;

    cout << "Running" << endl;
    TestPeriodic p(100);
    this_thread::sleep_for(4500ms);
*/
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
