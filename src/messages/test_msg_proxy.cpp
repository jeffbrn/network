#include "messages/test_msg_proxy.h"
#include "messages/test_generated.h"

using namespace std;

namespace netw::messages {

    TstMsgProxy::TstMsgProxy(const TestMsg *o) {
        id = o->id();
        posn = *o->posn();
        name = *o->name();
        is_valid = o->is_valid();
        buff.resize(o->buff()->size());
        memcpy(buff.data(), o->buff()->Data(), o->buff()->size());
    }


    void TstMsgProxy::Serialize(flatbuffers::FlatBufferBuilder& builder, const TstMsgProxy& tgt) {
    auto name = builder.CreateString(tgt.name);
    auto buff = builder.CreateVector(tgt.buff);
    TestMsgBuilder tgt_obj(builder);
    tgt_obj.add_id(tgt.id);
    tgt_obj.add_posn(&tgt.posn);
    tgt_obj.add_name(name);
    tgt_obj.add_is_valid(tgt.is_valid);
    tgt_obj.add_buff(buff);
    auto item = tgt_obj.Finish();
    builder.Finish(item);
}

TstMsgProxy TstMsgProxy::Deserialize(uint8_t* buff) {
    auto p = GetTestMsg(buff);
    if (p == nullptr) throw runtime_error("Unable to deserialize");
    return TstMsgProxy(p);
}

std::ostream& operator<<(std::ostream& os, const TstMsgProxy& o) {
    os << "TstMsgProxy: " << o.id << ", '" << o.name << "'" << endl;
    os << "    valid=" << o.is_valid << ", posn=(" << o.posn.x() << "," << o.posn.y() << "," << o.posn.z() << ")" << endl;
    os << "    buff(" << o.buff.size() << "): ";
    for(int i = 0; i < min(10, static_cast<int>(o.buff.size())); i++) {
        cout << (int)o.buff[i] << ", ";
    }
    return os;
}

}