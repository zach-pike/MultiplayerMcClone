#pragma once

#include <array>

#include "Block/Block.hpp"
#include "GameTypes/GameTypes.hpp"

#include <vector>

#define ChunkIndexFormula(csx, csz, x, y, z) (x + z * csx + y * csx * csz)
#define ReverseChunkIndexFormula(csx, csz, idx) glm::vec3( \
    (idx % csx), \
    (idx / (csx * csz)), \
    ((idx / csx) % csz) \
)

class Chunk {
public:
    static const int ChunkX = 16;
    static const int ChunkY = 16;
    static const int ChunkZ = 16;
    static const int ChunkBlockCount = ChunkX * ChunkY * ChunkZ;

    static const std::size_t ChunkSerializedSize = ChunkBlockCount * sizeof(Block);
    using BlockArray = std::array<Block, ChunkX * ChunkY * ChunkZ>;
protected:
    BlockArray blocks;
public:
    Chunk();
    virtual ~Chunk();

    void setBlock(Position pos, Block block);
    Block getBlock(Position pos);

    std::vector<std::uint8_t> serialize();
    void deserialize(const std::uint8_t*);

    const BlockArray& getBlocks() const;
};