#include "SCDisconnect.hpp"

#include <cstring>

namespace MessageTypes {

SCDisconnect::SCDisconnect() {}
SCDisconnect::~SCDisconnect() {}

std::string SCDisconnect::getReason() const {
    return reason;
}

void SCDisconnect::setReason(std::string r) {
    reason = r;
}

std::vector<std::uint8_t> SCDisconnect::serialize() const {
    std::vector<std::uint8_t> data;

    // Serialize the 'reason' field
    std::uint32_t reasonLength = static_cast<std::uint32_t>(reason.size());
    data.resize(sizeof(reasonLength));
    std::memcpy(data.data(), &reasonLength, sizeof(reasonLength));

    data.insert(data.end(), reason.begin(), reason.end());

    return data;
}

bool SCDisconnect::deserialize(const std::vector<std::uint8_t>& data) {
    if (data.size() < sizeof(std::uint32_t)) {
        return false; // Not enough data to deserialize
    }

    std::size_t offset = 0;

    // Deserialize the 'reason' field
    std::uint32_t reasonLength;
    std::memcpy(&reasonLength, data.data() + offset, sizeof(reasonLength));
    offset += sizeof(reasonLength);

    if (offset + reasonLength > data.size()) {
        return false; // Not enough data to deserialize reason
    }

    reason.assign(reinterpret_cast<const char*>(data.data() + offset), reasonLength);

    return true;
}

GameMessageType SCDisconnect::getGameMessageType() const {
    return GameMessageType::SC_Disconnect;
}

}