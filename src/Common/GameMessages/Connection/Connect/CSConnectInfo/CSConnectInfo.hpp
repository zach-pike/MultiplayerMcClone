#pragma once

#include "Common/GameMessages/GameMessageTypes.hpp"

namespace MessageTypes {

class CSConnectInfo : public GameMessage {
private:
    std::string username;
public:
    CSConnectInfo();
    ~CSConnectInfo();

    std::string getUsername() const;
    void setUsername(std::string name);

    std::vector<std::uint8_t> serialize() const;
    bool deserialize(const std::vector<std::uint8_t>& data);

    GameMessageType getGameMessageType() const;
};

}