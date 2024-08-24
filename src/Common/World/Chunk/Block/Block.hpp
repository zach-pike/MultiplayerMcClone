#pragma once

#include <cstdint>


struct Block {
    using BlockIDType = std::uint32_t;

    BlockIDType blockID;

    Block();
    Block(BlockIDType b);
};