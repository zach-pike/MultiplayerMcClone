#include "Chunk.hpp"

#include <exception>
#include <stdexcept>

#include <cstring>

Chunk::Chunk() {}
Chunk::~Chunk() {}

void Chunk::setBlock(Position pos, Block block) {
    blocks.at(ChunkIndexFormula(Chunk::ChunkX, Chunk::ChunkZ, pos.x, pos.y, pos.z)) = block;
}

Block Chunk::getBlock(Position pos) {
    return blocks.at(ChunkIndexFormula(Chunk::ChunkX, Chunk::ChunkZ, pos.x, pos.y, pos.z));
}

std::vector<std::uint8_t> Chunk::serialize() {
    return std::vector<std::uint8_t>(
        reinterpret_cast<std::uint8_t*>(blocks.data()),
        reinterpret_cast<std::uint8_t*>(blocks.data()) + blocks.size() * sizeof(Block)
    );
}

void Chunk::deserialize(const std::uint8_t* data) {
    if (data == nullptr) {
        throw std::invalid_argument("Null data pointer passed to Chunk::deserialize.");
    }

    // Assuming that the data buffer is guaranteed to be at least ChunkSerializedSize bytes
    std::memcpy(blocks.data(), data, ChunkSerializedSize);
}

const Chunk::BlockArray& Chunk::getBlocks() const {
    return blocks;
}