#include "Chunk.hpp"

#include <exception>
#include <stdexcept>

Chunk::Chunk() {}
Chunk::~Chunk() {}

void Chunk::setBlock(Position pos, Block block) {
    blocks.at(pos.x + pos.z * Chunk::ChunkX + pos.y * Chunk::ChunkX * Chunk::ChunkZ) = block;
}

Block Chunk::getBlock(Position pos) {
    return blocks.at(pos.x + pos.z * Chunk::ChunkX + pos.y * Chunk::ChunkX * Chunk::ChunkZ);
}

std::vector<std::uint8_t> Chunk::serialize() {
    return std::vector<std::uint8_t>((std::uint8_t*)blocks.data(),
                                     (std::uint8_t*)blocks.data() + blocks.size() * sizeof(Block));
}

void Chunk::deserialize(const std::uint8_t* data) {
    std::copy((Block*)data, (Block*)data + blocks.size(), blocks.begin());
}