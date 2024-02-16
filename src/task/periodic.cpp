#include "task/periodic.h"

using namespace std;

namespace task {

    Periodic::Periodic(int period_ms) :
        thrd_{[&] { this->thrd_main(); }},
        running_{true},
        period_{chrono::milliseconds(period_ms)}
        {
        }

    Periodic::~Periodic() {
        running_ = false;
        thrd_.join();
    }

    void Periodic::thrd_main() {
        last_ = std::chrono::steady_clock::now();
        using namespace std::chrono_literals;
        while (running_) {
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - last_);
            std::this_thread::sleep_for(duration - period_);
            if (duration >= period_) {
                std::cout << "*** Triggered ***  " << duration.count() << std::endl;
                job();
                last_ = std::chrono::steady_clock::now();
            }
        }
        cout << "Thread exit" << endl;
    }

}
