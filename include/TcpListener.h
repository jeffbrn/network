#ifndef NETWORK_TCPLISTENER_H
#define NETWORK_TCPLISTENER_H

#include <thread>

namespace network {

    class TcpListener {
    public:
        TcpListener(int portnum);
        ~TcpListener();

    private:
        int skt_fd_ = -1;
        std::thread thrd_;
        static constexpr int BUFF_SZ = 1024;
        uint8_t buff_[BUFF_SZ];

        void handler();
    };
}

#endif //NETWORK_TCPLISTENER_H
