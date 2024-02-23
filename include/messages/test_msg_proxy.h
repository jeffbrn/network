#ifndef NETWORK_TEST_MSG_PROXY_H
#define NETWORK_TEST_MSG_PROXY_H

#include <cstdint>
#include <string>
#include <utility>
#include <vector>
#include "test_generated.h"
#include <iostream>

namespace netw::messages {

    class TstMsgProxy {
    public:
        TstMsgProxy() = default;
        TstMsgProxy(uint64_t id_, Vec3 posn_, std::string name_, bool valid_, std::vector<uint8_t> buff_) :
            id{id_}, posn{posn_}, name{std::move(name_)}, is_valid{valid_}, buff{std::move(buff_)}
        {}

        uint64_t id {0};
        Vec3 posn;
        std::string name;
        bool is_valid {false};
        std::vector<uint8_t> buff;

        static void Serialize(flatbuffers::FlatBufferBuilder&, const TstMsgProxy&);
        static TstMsgProxy Deserialize(uint8_t*);

    private:
        explicit TstMsgProxy(const TestMsg *o);
    };

    std::ostream& operator<<(std::ostream& os, const TstMsgProxy& o);

}

#endif //NETWORK_TEST_MSG_PROXY_H
