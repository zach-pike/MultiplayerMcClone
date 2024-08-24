#include "Chunk.hpp"

#include <exception>
#include <stdexcept>

Chunk::Chunk() {}
Chunk::~Chunk() {}

void Chunk::setBlock(Position pos, Block block) {
    blocks.at(ChunkIndexFormula(Chunk::ChunkX, Chunk::ChunkZ, pos.x, pos.y, pos.z)) = block;
}

Block Chunk::getBlock(Position pos) {
    return blocks.at(ChunkIndexFormula(Chunk::ChunkX, Chunk::ChunkZ, pos.x, pos.y, pos.z));
}

std::vector<std::uint8_t> Chunk::serialize() {
    return std::vector<std::uint8_t>((std::uint8_t*)blocks.data(),
                                     (std::uint8_t*)blocks.data() + blocks.size() * sizeof(Block));
}

void Chunk::deserialize(const std::uint8_t* data) {
    std::copy((Block*)data, (Block*)data + blocks.size(), blocks.begin());
}