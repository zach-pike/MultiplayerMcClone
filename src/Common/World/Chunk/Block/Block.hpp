#pragma once

#include <cstdint>


struct Block {
    enum class Face : std::uint8_t {
        NegativeX = 0,
        PositiveX = 1,

        NegativeY = 2,
        PositiveY = 3,

        NegativeZ = 4,
        PositiveZ = 5,
    };

    using BlockIDType = std::uint32_t;

    BlockIDType blockID;

    Block();
    Block(BlockIDType b);

    bool operator==(const Block&) const;
};