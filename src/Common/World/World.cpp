#include "World.hpp"

#include <cstring>

World::SerializedWorldInformation::SerializedWorldInformation() {}

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

    // Create chunk positions array and chunk data vector
    std::unique_ptr<int[]> chunkPositions(new int[chunks.size() * 3]);
    std::vector<std::vector<std::uint8_t>> chunkData(chunks.size());

    int i = 0;
    for (const auto& kv : chunks) {
        const auto& pos = kv.first;

        chunkPositions[i * 3 + 0] = pos.x;
        chunkPositions[i * 3 + 1] = pos.y;
        chunkPositions[i * 3 + 2] = pos.z;

        chunkData[i] = kv.second->serialize();
        i++;
    }

    SerializedWorldInformation s(static_cast<int>(chunks.size()));

    std::vector<std::uint8_t> finalByteArray;

    // Header
    finalByteArray.insert(
        finalByteArray.end(),
        reinterpret_cast<std::uint8_t*>(&s),
        reinterpret_cast<std::uint8_t*>(&s) + sizeof(SerializedWorldInformation)
    );

    // Chunk positions
    finalByteArray.insert(
        finalByteArray.end(),
        reinterpret_cast<std::uint8_t*>(chunkPositions.get()),
        reinterpret_cast<std::uint8_t*>(chunkPositions.get()) + (chunks.size() * 3 * sizeof(int))
    );

    // Append each chunk to the end
    for (const auto& chunk : chunkData) {
        finalByteArray.insert(finalByteArray.end(), chunk.begin(), chunk.end());
    }

    return finalByteArray;
}

void World::deserialize(const std::vector<std::uint8_t>& data) {
    const std::uint8_t* ptr = data.data();

    // Read the header (SerializedWorldInformation)
    SerializedWorldInformation s;
    std::memcpy(&s, ptr, sizeof(SerializedWorldInformation));
    ptr += sizeof(SerializedWorldInformation);

    // Allocate memory for chunk positions
    std::unique_ptr<ChunkCoordinates[]> chunkPositions(new ChunkCoordinates[s.numberOfChunks]);

    // Read chunk positions
    for (int i = 0; i < s.numberOfChunks; i++) {
        int x, y, z;
        std::memcpy(&x, ptr, sizeof(int));
        ptr += sizeof(int);

        std::memcpy(&y, ptr, sizeof(int));
        ptr += sizeof(int);

        std::memcpy(&z, ptr, sizeof(int));
        ptr += sizeof(int);

        chunkPositions[i] = ChunkCoordinates{x, y, z};
    }

    // Read and deserialize each chunk
    for (int i = 0; i < s.numberOfChunks; i++) {
        auto c = std::make_shared<Chunk>();
        c->deserialize(ptr);  // Assuming Chunk::deserialize reads the data directly from the pointer
        ptr += Chunk::ChunkSerializedSize;

        chunks.insert({chunkPositions[i], std::move(c)});
    }

    // No need to delete chunkPositions since it is managed by std::unique_ptr
}

void World::generateWorld() {
    for (int cx=0; cx < 3; cx++) {
        for (int cz=0; cz < 3; cz++) {
            for (int cy=0; cy < 3; cy++) {
                // Make one chunk
                auto c = std::make_shared<Chunk>();

                for (int x=0; x<16; x++) {
                    for (int z=0; z<16; z++) {
                        for (int y=0; y<15; y++) {
                            c->setBlock(Position{ x, y, z }, Block(1));
                        }
                    }
                }

                for (int x=0; x<16; x++) {
                    for (int z=0; z<16; z++) {
                        c->setBlock(Position{ x, 15, z }, Block(2));
                    }
                }

                chunks[ChunkCoordinates{ cx, cy, cz }] = std::move(c);
            }
        }
    }
}

const std::map<ChunkCoordinates, std::shared_ptr<Chunk>>& World::getChunks() const {
    return chunks;
}