#ifndef NETWORK_TCPCLIENT_H
#define NETWORK_TCPCLIENT_H

#include <utility>
#include <cstdint>

namespace network {

    class TcpClient {
    public:
        TcpClient(const char *, const char *);

        ~TcpClient();

        void send(const char *, uint16_t);

    private:
        int skt_fd_ = -1;
        static constexpr int BUFF_SZ = 65536;
        char buff_[BUFF_SZ]{};
    };

}
#endif //NETWORK_TCPCLIENT_H
