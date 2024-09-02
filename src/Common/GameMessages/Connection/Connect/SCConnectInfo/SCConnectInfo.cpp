#include "SCConnectInfo.hpp"

#include <stdexcept>

namespace MessageTypes {

SCConnectInfo::SCConnectInfo() {}
SCConnectInfo::~SCConnectInfo() {}

std::vector<std::uint8_t> SCConnectInfo::serialize() const {
    return std::vector<std::uint8_t>();
}

bool SCConnectInfo::deserialize(const std::vector<std::uint8_t>& data) {
    return true;
}

GameMessageType SCConnectInfo::getGameMessageType() const {
    return GameMessageType::SC_Connect_Info;
}

}