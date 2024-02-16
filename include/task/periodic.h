#ifndef TASK_PERIODIC_H
#define TASK_PERIODIC_H

#include <thread>
#include <atomic>
#include <chrono>
#include <iostream>

namespace task {

    class Periodic {
    public:
        explicit Periodic(int period_ms);
        virtual ~Periodic();

    protected:
        virtual void job() = 0;

    private:
        std::atomic<bool> running_;
        std::thread thrd_;
        std::chrono::time_point<std::chrono::steady_clock> last_;
        std::chrono::milliseconds period_;

        void thrd_main();
    };

}

#endif //TASK_PERIODIC_H
