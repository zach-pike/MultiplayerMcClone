#include "MessagePreprocessor.hpp"

#include <stdexcept>

Message processMessage(std::shared_ptr<MessageTypes::GameMessage> message, bool compression) {
    // Get id
    MessageIDType id = (MessageIDType)message->getGameMessageType();

    // Data
    std::vector<std::uint8_t> data = message->serialize();

    if (compression) {

    } else {
        return Message(id, data);
    }

    throw std::runtime_error("error");
}