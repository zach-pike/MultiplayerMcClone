#include "SCPlayerData.hpp"

#include "Common/Serialize/Serializer.hpp"

namespace MessageTypes {

SCPlayerData::SCPlayerData() {}
SCPlayerData::~SCPlayerData() {}

const std::vector<std::string>& SCPlayerData::getPlayerUsernames() const {
    return playerUsernames;
}

void SCPlayerData::setPlayerUsernames(const std::vector<std::string>& d) {
    playerUsernames = d;
}

const std::vector<Vec3fPosition>& SCPlayerData::getPlayerPositions() {
    return playerPositions;
}

void SCPlayerData::setPlayerPositions(const std::vector<Vec3fPosition>& p) {
    playerPositions = p;
}

std::vector<std::uint8_t> SCPlayerData::serialize() const {
    Serializer s;
    s << playerPositions << playerUsernames;
    return s.getBytes();
}

bool SCPlayerData::deserialize(const std::vector<std::uint8_t>& data) {
    Serializer s(data);
    s >> playerUsernames >> playerPositions;
    return true;
}

GameMessageType SCPlayerData::getGameMessageType() const {
    return GameMessageType::SC_Player_Data;
}

}