#include "messages/test_msg_proxy.h"
#include "messages/test_generated.h"

using namespace std;

namespace messages {

void TstMsgProxy::Serialize(flatbuffers::FlatBufferBuilder& builder, const TstMsgProxy& tgt) {
    TestMsgBuilder tgt_obj(builder);
    auto name = builder.CreateString(tgt.name);
    auto buff = builder.CreateVector(tgt.buff);
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

}