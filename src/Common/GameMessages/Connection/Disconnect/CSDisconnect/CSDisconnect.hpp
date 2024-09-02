#pragma once

#include "Common/GameMessages/GameMessageTypes.hpp"

namespace MessageTypes {

class CSDisconnect : public GameMessage {
private:
    std::string reason;
public:
    CSDisconnect();
    ~CSDisconnect();


    std::string getReason() const;
    void setReason(std::string r);

    std::vector<std::uint8_t> serialize() const;
    bool deserialize(const std::vector<std::uint8_t>& data);

    GameMessageType getGameMessageType() const;
};

}
