#ifndef NETWORK_TEST_MSG_PROXY_H
#define NETWORK_TEST_MSG_PROXY_H

#include <cstdint>
#include <string>
#include <vector>
#include "test_generated.h"

namespace messages {

    class TstMsgProxy {
    public:
        TstMsgProxy() = default;
        uint64_t id {0};
        Vec3 posn;
        std::string name;
        bool is_valid {false};
        std::vector<uint8_t> buff;

        static void Serialize(flatbuffers::FlatBufferBuilder&, const TstMsgProxy&);
        static TstMsgProxy Deserialize(uint8_t*);

    private:
        explicit TstMsgProxy(const TestMsg *o) {
            id = o->id();
            posn = *o->posn();
            name = *o->name();
            is_valid = o->is_valid();
            std::copy(std::begin(*o->buff()), std::end(*o->buff()), std::begin(buff));
        }
    };

}

#endif //NETWORK_TEST_MSG_PROXY_H
