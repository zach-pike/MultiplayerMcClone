#include "CSUpdatePlayerInfo.hpp"
#include "Common/Serialize/Serializer.hpp"
#include <cstring>

namespace MessageTypes {

CSUpdatePlayerInfo::CSUpdatePlayerInfo() {}
CSUpdatePlayerInfo::~CSUpdatePlayerInfo() {}

Vec3fPosition CSUpdatePlayerInfo::getPosition() const {
    return newPosition;
}

void CSUpdatePlayerInfo::setPosition(Vec3fPosition p) {
    newPosition = p;
}

std::vector<std::uint8_t> CSUpdatePlayerInfo::serialize() const {
    Serializer s;
    s.preReserve(sizeof(Vec3fPosition));
    s << newPosition;
    return s.getBytes();
}

bool CSUpdatePlayerInfo::deserialize(const std::vector<std::uint8_t>& data) {
    Serializer s(data);
    s >> newPosition;

    return true;
}

GameMessageType CSUpdatePlayerInfo::getGameMessageType() const {
    return GameMessageType::CS_Update_Player_Info;
}

}