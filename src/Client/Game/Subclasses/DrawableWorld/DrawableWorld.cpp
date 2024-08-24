#include "DrawableWorld.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include "DrawableChunk/DrawableChunk.hpp"

DrawableWorld::DrawableWorld() {}
DrawableWorld::~DrawableWorld() {}

void DrawableWorld::drawWorld() {
    for (auto& kv : World::chunks) {
        auto c = std::dynamic_pointer_cast<DrawableChunk>(kv.second);
        c->drawChunk(kv.first, *this);
    }
}

void DrawableWorld::renderWorld(std::unique_ptr<WorldRenderInfo>& renderInfo, glm::mat4 viewProjection) {
    for (auto& kv : World::chunks) {
        auto& pos = kv.first;
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(pos.x, pos.y, pos.z)*glm::vec3(Chunk::ChunkX, Chunk::ChunkY, Chunk::ChunkZ));
    
        // Upcast chunk to DrawableChunk
        std::shared_ptr<DrawableChunk> c = std::dynamic_pointer_cast<DrawableChunk>(kv.second);
        c->renderChunk(renderInfo, viewProjection * modelMatrix);
    }
}

void DrawableWorld::generateWorld() {
    for (int cx=0; cx < 3; cx++) {
        for (int cz=0; cz < 3; cz++) {
            for (int cy=0; cy < 3; cy++) {
                // Make one chunk
                auto c = std::make_shared<DrawableChunk>();

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