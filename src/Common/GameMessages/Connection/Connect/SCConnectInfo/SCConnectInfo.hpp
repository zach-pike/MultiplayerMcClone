#pragma once

#include "Common/GameMessages/GameMessageTypes.hpp"

namespace MessageTypes {

class SCConnectInfo : public GameMessage {
private:
public:
    SCConnectInfo();
    ~SCConnectInfo();

    std::vector<std::uint8_t> serialize() const;
    bool deserialize(const std::vector<std::uint8_t>& data);

    GameMessageType getGameMessageType() const;
};

}
