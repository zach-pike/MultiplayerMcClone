#include "DrawableWorld.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include "DrawableChunk/DrawableChunk.hpp"

void DrawableWorld::convertChunksToDrawableChunks() {
    for (auto& currentChunk : chunks) {
        // Create a DrawableChunk
        auto newChunk = std::make_shared<DrawableChunk>(currentChunk.second);

        // release the old chunk
        currentChunk.second.reset();
        chunks[currentChunk.first] = newChunk;
    }
}

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
    World::generateWorld();

    convertChunksToDrawableChunks();
}

void DrawableWorld::deserialize(const std::vector<std::uint8_t>& d) {
    World::deserialize(d);

    convertChunksToDrawableChunks();
}