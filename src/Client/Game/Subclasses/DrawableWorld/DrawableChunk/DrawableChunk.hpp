#pragma once

#include "glad/glad.h"

#include "Common/World/Chunk/Chunk.hpp"

#include <memory>
#include "Game/WorldRenderInfo/WorldRenderInfo.hpp"

#include "Common/GameTypes/GameTypes.hpp"

#include <glm/glm.hpp>

class DrawableWorld;

class DrawableChunk : public Chunk {
private:
    GLuint vertexIndexBuffer, vertexPositionBuffer, vertexTextureBuffer;

    bool meshUpToDate = false;

    std::size_t lastVertexCount = 0;

    void initBuffers();
    void uninitBuffers();
public:
    DrawableChunk();
    DrawableChunk(const std::shared_ptr<Chunk>& c);
    ~DrawableChunk();

    void drawChunk(ChunkCoordinates cc, const DrawableWorld&);
    void renderChunk(std::unique_ptr<WorldRenderInfo>& renderInfo, glm::mat4 MVP);
};