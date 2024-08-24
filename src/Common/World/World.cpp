#include "World.hpp"

#include <string.h>

World::SerializedWorldInformation::SerializedWorldInformation(int _nChunks) {
    numberOfChunks = _nChunks;
}

World::World() {}

World::~World() {}

void World::setBlock(Position pos, Block block) {
    // get chunk position
    ChunkCoordinates c = { pos.x / Chunk::ChunkX,
                           pos.y / Chunk::ChunkY,
                           pos.z / Chunk::ChunkZ };

    Position p = { pos.x % Chunk::ChunkX,
                   pos.y % Chunk::ChunkY,
                   pos.z % Chunk::ChunkZ };
    
    chunks.at(c)->setBlock(p, block);
}

Block World::getBlock(Position pos) const {
    // get chunk position
    ChunkCoordinates c = { pos.x / Chunk::ChunkX,
                           pos.y / Chunk::ChunkY,
                           pos.z / Chunk::ChunkZ };

    Position p = { pos.x % Chunk::ChunkX,
                   pos.y % Chunk::ChunkY,
                   pos.z % Chunk::ChunkZ };

    return chunks.at(c)->getBlock(p);
}

std::vector<std::uint8_t> World::serialize() {
    // ORDER OF SERIALIZED FORM

    // SerializedWorldInformation
    // chunk positions array
    // chunks array

    // Make a table
    int* chunkPositions = new int[chunks.size() * 3];
    std::vector<std::uint8_t>* chunkData = new std::vector<std::uint8_t>[chunks.size()];

    int i=0;
    for (auto& kv : chunks) {
        auto pos = kv.first;

        chunkPositions[i * 3 + 0] = pos.x;
        chunkPositions[i * 3 + 1] = pos.y;
        chunkPositions[i * 3 + 2] = pos.z;

        chunkData[i] = kv.second->serialize();

        i++;
    }

    SerializedWorldInformation s(chunks.size());

    std::vector<std::uint8_t> finalByteArray;

    // Header
    finalByteArray.insert(finalByteArray.end(), (std::uint8_t*)&s, (std::uint8_t*)&s + sizeof(SerializedWorldInformation));

    // Chunk positions
    finalByteArray.insert(finalByteArray.end(), (std::uint8_t*)&chunkPositions, (std::uint8_t*)&chunkPositions + (chunks.size() * 3 * sizeof(int)));
    
    // Append each chunk to the end
    for (int i=0; i<chunks.size(); i++) {
        finalByteArray.insert(finalByteArray.end(), chunkData[i].begin(), chunkData[i].end());
    }

    delete[] chunkData;
    delete[] chunkPositions;

    return finalByteArray;
}

void World::deserialize(const std::vector<std::uint8_t>& data) {
    const std::uint8_t* ptr = data.data();

    // Read the header in
    SerializedWorldInformation s = *(SerializedWorldInformation*)ptr;
    ptr += sizeof(SerializedWorldInformation);
    
    ChunkCoordinates* chunkPositions = new ChunkCoordinates[s.numberOfChunks];
    std::uint8_t* chunkData = new std::uint8_t[s.numberOfChunks * Chunk::ChunkSerializedSize];

    for (int i=0; i<s.numberOfChunks; i++) {
        // read chunk
        int x = *(int*)ptr;
        ptr += sizeof(int);

        int y = *(int*)ptr;
        ptr += sizeof(int);

        int z = *(int*)ptr;
        ptr += sizeof(int);

        chunkPositions[i] = ChunkCoordinates { x, y, z };
    }

    // Pointer is now advanced to the chunk data section
    for (int i=0; i<s.numberOfChunks; i++) {
        memcpy(&chunkData[i * Chunk::ChunkSerializedSize], ptr, Chunk::ChunkSerializedSize);
        ptr += Chunk::ChunkSerializedSize;
    }

    // Insert chunks into world
    for (int i=0; i<s.numberOfChunks; i++) {
        auto c = std::make_shared<Chunk>();
        c->deserialize(&chunkData[i * Chunk::ChunkSerializedSize]);
        
        chunks.insert({ chunkPositions[i], std::move(c) });
    }

    delete[] chunkPositions;
    delete[] chunkData;
}

void World::generateWorld() {
    // Make one chunk
    Chunk c;
    for (int x=0; x<16; x++) {
        for (int z=0; z<16; z++) {
            for (int y=0; y<15; y++) {
                c.setBlock(Position{ x, y, z }, Block(0));
            }
        }
    }

    for (int x=0; x<16; x++) {
        for (int z=0; z<16; z++) {
            c.setBlock(Position{ x, 15, z }, Block(1));
        }
    }
}

const std::map<ChunkCoordinates, std::shared_ptr<Chunk>>& World::getChunks() const {
    return chunks;
}