#pragma once

#include "Chunk/Chunk.hpp"
#include "GameTypes/GameTypes.hpp"

#include <map>
#include <memory>
#include <vector>

class World {
public:
    struct SerializedWorldInformation {
        int numberOfChunks;

        SerializedWorldInformation(int nChunks);
    };
protected:
    std::map<ChunkCoordinates, std::shared_ptr<Chunk>> chunks;
public:
    World();
    ~World();

    virtual void setBlock(Position pos, Block blockType);
    virtual Block getBlock(Position pos) const;

    std::vector<std::uint8_t> serialize();
    void deserialize(const std::vector<std::uint8_t>&);

    virtual void generateWorld();

    const std::map<ChunkCoordinates, std::shared_ptr<Chunk>>& getChunks() const;
};