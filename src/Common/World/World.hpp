#pragma once

#include "Chunk/Chunk.hpp"
#include "GameTypes/GameTypes.hpp"

#include <map>
#include <vector>

class World {
public:
    struct SerializedWorldInformation {
        int numberOfChunks;

        SerializedWorldInformation(int nChunks);
    };
private:
    std::map<ChunkCoordinates, Chunk> chunks;
public:
    World();
    ~World();

    void setBlock(Position pos, Block blockType);
    Block getBlock(Position pos);

    std::vector<std::uint8_t> serialize();
    void deserialize(const std::vector<std::uint8_t>&);

    void generateWorld();
};