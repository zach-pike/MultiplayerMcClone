#pragma once

#include "Common/GameMessages/GameMessageTypes.hpp"

namespace MessageTypes {

class SCDisconnect : public GameMessage {
private:
    std::string reason;
public:
    SCDisconnect();
    ~SCDisconnect();

    std::string getReason() const;
    void setReason(std::string r);

    std::vector<std::uint8_t> serialize() const;
    bool deserialize(const std::vector<std::uint8_t>& data);

    GameMessageType getGameMessageType() const;
};

}
