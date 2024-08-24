#include "Block.hpp"

Block::Block():
    blockID(0) {}

Block::Block(BlockIDType b) {
    blockID = b;
}

bool Block::operator==(const Block& block) const {
    return block.blockID == blockID;
}