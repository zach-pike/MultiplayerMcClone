#include "CSConnectInfo.hpp"

#include "Common/Serialize/Serializer.hpp"

#include <cstring>
#include <stdexcept>

namespace MessageTypes {

CSConnectInfo::CSConnectInfo() {}
CSConnectInfo::~CSConnectInfo() {}

std::string CSConnectInfo::getUsername() const {
    return username;
}

void CSConnectInfo::setUsername(std::string nname) {
    username = nname;
}

std::vector<std::uint8_t> CSConnectInfo::serialize() const {
    Serializer s;

    s << username;

    return s.getBytes();
}

bool CSConnectInfo::deserialize(const std::vector<std::uint8_t>& data) {
    Serializer s(data);
    s >> username;
    return true;
}

GameMessageType CSConnectInfo::getGameMessageType() const {
    return GameMessageType::CS_Connect_Info;
}

}