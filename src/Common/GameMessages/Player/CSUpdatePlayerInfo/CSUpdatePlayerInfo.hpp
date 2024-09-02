#pragma once

#include "Common/GameMessages/GameMessageTypes.hpp"

namespace MessageTypes {

class CSUpdatePlayerInfo : public GameMessage {
private:
    Vec3fPosition newPosition;
public:
    CSUpdatePlayerInfo();
    ~CSUpdatePlayerInfo();

    Vec3fPosition getPosition() const;
    void setPosition(Vec3fPosition p);

    std::vector<std::uint8_t> serialize() const;
    bool deserialize(const std::vector<std::uint8_t>& d);

    GameMessageType getGameMessageType() const;
};

}