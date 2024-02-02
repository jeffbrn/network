#include <iostream>

#include "TcpListener.h"

using namespace std;

int main() {
    cout << "Test Echo server" << endl << endl;

    network::TcpListener svr(5151);
    svr.set_msg_handler([] (const uint8_t* buff, uint32_t len) {
        for(uint32_t i = 0; i < len; i++) {
            auto ch = (int)buff[i];
            cout << ch << ',';
        }
        cout << endl;
    });
    svr.wait();

    cout << "Done" << endl;
    return 0;
}
