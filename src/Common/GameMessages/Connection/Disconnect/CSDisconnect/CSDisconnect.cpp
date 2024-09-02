#include "CSDisconnect.hpp"

#include <cstring>

namespace MessageTypes {

CSDisconnect::CSDisconnect() {}
CSDisconnect::~CSDisconnect() {}

std::string CSDisconnect::getReason() const {
    return reason;
}

void CSDisconnect::setReason(std::string r) {
    reason = r;
}

std::vector<std::uint8_t> CSDisconnect::serialize() const {


    return std::vector<std::uint8_t>();
}

bool CSDisconnect::deserialize(const std::vector<std::uint8_t>& data) {
    

    return true;
}

GameMessageType CSDisconnect::getGameMessageType() const {
    return GameMessageType::CS_Disconnect;
}

}