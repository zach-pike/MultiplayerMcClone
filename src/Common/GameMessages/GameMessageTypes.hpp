#pragma once

#include <cstdint>

#include "Common/MessageTypes.hpp"

#include "Common/Serialize/Serializable.hpp"

namespace MessageTypes {

enum class GameMessageType : MessageIDType {
    SC_Connect_Info      = 0,
    SC_Disconnect        = 1,
    SC_Player_Data  = 2,
    SC_Load_Chunks       = 3,

    // Has UUID field attached
    CS_Connect_Info      = 4,
    CS_Disconnect        = 5,
    CS_Update_Player_Info = 6,
};

class GameMessage : public Serializable {
public:
    virtual GameMessageType getGameMessageType() const = 0;
};

};