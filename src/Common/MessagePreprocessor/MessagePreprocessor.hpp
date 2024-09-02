#pragma once

#include "Common/MessageTypes.hpp"
#include "Common/GameMessages/GameMessageTypes.hpp"

#include <memory>

Message processMessage(std::shared_ptr<MessageTypes::GameMessage> message, bool compression = false);