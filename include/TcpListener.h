#ifndef NETWORK_TCPLISTENER_H
#define NETWORK_TCPLISTENER_H

#include <thread>
#include <atomic>
#include <functional>

namespace network {

    class TcpListener {
    public:
        explicit TcpListener(uint16_t portnum);
        ~TcpListener();

        void set_msg_handler(std::function<void (const char*, uint16_t)>);
        void wait();

    private:
        int skt_fd_ = -1;
        uint16_t port_;
        std::thread thrd_;
        std::atomic_flag running_ = ATOMIC_FLAG_INIT;
        static constexpr int BUFF_SZ = 65536;
        char buff_[BUFF_SZ] {};

        void handler();
        std::function<void (const char*, uint16_t)> msg_handler_ {};
    };
}

#endif //NETWORK_TCPLISTENER_H
