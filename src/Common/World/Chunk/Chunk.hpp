#pragma once

#include <array>

#include "Block/Block.hpp"
#include "GameTypes/GameTypes.hpp"

#include <vector>

class Chunk {
public:
    static const int ChunkX = 16;
    static const int ChunkY = 16;
    static const int ChunkZ = 16;

    static const std::size_t ChunkSerializedSize = ChunkX *ChunkY * ChunkZ * sizeof(Block);
private:
    std::array<Block, ChunkX * ChunkY * ChunkZ> blocks;
public:
    Chunk();
    ~Chunk();

    void setBlock(Position pos, Block block);
    Block getBlock(Position pos);

    std::vector<std::uint8_t> serialize();
    void deserialize(const std::uint8_t*);
};