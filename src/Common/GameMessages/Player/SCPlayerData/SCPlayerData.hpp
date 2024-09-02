#pragma once

#include "Common/GameMessages/GameMessageTypes.hpp"

namespace MessageTypes {

class SCPlayerData : public GameMessage {
private:
    std::vector<std::string>   playerUsernames;
    std::vector<Vec3fPosition> playerPositions;
public:
    SCPlayerData();
    ~SCPlayerData();

    const std::vector<std::string>& getPlayerUsernames() const;
    void setPlayerUsernames(const std::vector<std::string>&);

    const std::vector<Vec3fPosition>& getPlayerPositions();
    void setPlayerPositions(const std::vector<Vec3fPosition>& p);

    std::vector<std::uint8_t> serialize() const;
    bool deserialize(const std::vector<std::uint8_t>& data);

    GameMessageType getGameMessageType() const;
};

}
