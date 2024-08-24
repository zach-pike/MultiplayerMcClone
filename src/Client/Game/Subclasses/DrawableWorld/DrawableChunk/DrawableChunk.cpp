#include "DrawableChunk.hpp"

#include "../DrawableWorld.hpp"

static const Block::Face cullingFaces[] = { Block::Face::NegativeX, Block::Face::PositiveX,
                                      Block::Face::NegativeY, Block::Face::PositiveY,
                                      Block::Face::NegativeZ, Block::Face::NegativeZ };

struct CullingOffset {
    int x, y, z;
};

static const CullingOffset cullingOffset[] = {
    CullingOffset{ -1, 0, 0 },
    CullingOffset{  1, 0, 0 },
    CullingOffset{ 0, -1, 0 },
    CullingOffset{ 0,  1, 0 },
    CullingOffset{ 0, 0, -1 },
    CullingOffset{ 0, 0,  1 },
};

template <typename T>
static T mod(T a, T b) {
    return (a % b + b) % b;
}

DrawableChunk::DrawableChunk() {
    glGenBuffers(1, &vertexIndexBuffer);
    glGenBuffers(1, &vertexPositionBuffer);
    glGenBuffers(1, &vertexTextureBuffer);
}

DrawableChunk::~DrawableChunk() {
    glDeleteBuffers(1, &vertexIndexBuffer);
    glDeleteBuffers(1, &vertexPositionBuffer);
    glDeleteBuffers(1, &vertexTextureBuffer);
}

void DrawableChunk::drawChunk(ChunkCoordinates cc, const DrawableWorld& world) {
    if (meshUpToDate) return;

    // Fun code
    std::vector<GLuint> vertexIndex;
    std::vector<glm::vec3> offsets;
    std::vector<GLuint> textureIndexs;

    auto addFace = [&](Block::Face face, glm::vec3 offset, GLuint textureIndex) {
        vertexIndex.push_back((GLuint)face*6 + 0);
        vertexIndex.push_back((GLuint)face*6 + 1);
        vertexIndex.push_back((GLuint)face*6 + 2);
        vertexIndex.push_back((GLuint)face*6 + 3);
        vertexIndex.push_back((GLuint)face*6 + 4);
        vertexIndex.push_back((GLuint)face*6 + 5);

        offsets.push_back(offset);
        offsets.push_back(offset);
        offsets.push_back(offset);
        offsets.push_back(offset);
        offsets.push_back(offset);
        offsets.push_back(offset);

        textureIndexs.push_back(textureIndex);
        textureIndexs.push_back(textureIndex);
        textureIndexs.push_back(textureIndex);
        textureIndexs.push_back(textureIndex);
        textureIndexs.push_back(textureIndex);
        textureIndexs.push_back(textureIndex);
    };

    auto addBlock = [&](glm::vec3 offset, GLuint textureIndex) {
        int worldX = cc.x * Chunk::ChunkX + offset.x;
        int worldY = cc.y * Chunk::ChunkY + offset.y;
        int worldZ = cc.z * Chunk::ChunkZ + offset.z;

        for (int i=0; i<6; i++) {
            CullingOffset off = cullingOffset[i];

            int checkX = offset.x + off.x;
            int checkY = offset.y + off.y;
            int checkZ = offset.z + off.z;

            if ((checkX >= 0 && checkX < Chunk::ChunkX) &&
                (checkY >= 0 && checkY < Chunk::ChunkY) &&
                (checkZ >= 0 && checkZ < Chunk::ChunkZ))
            {
                // Block to check is in current chunk
                if (getBlock(Position{ checkX, checkY, checkZ }) != Block(0)) continue;
            }

            // Now for outside check
            int newCheckX = mod(checkX, Chunk::ChunkX);
            int newCheckY = mod(checkY, Chunk::ChunkY);
            int newCheckZ = mod(checkZ, Chunk::ChunkZ);

            // Now get neighboring chunk (if it exists)
            auto checkingChunk = ChunkCoordinates{ cc.x + off.x, cc.y + off.y, cc.z + off.z };
            if (world.getChunks().count(checkingChunk)) {
                const auto& chunk = world.getChunks().at(checkingChunk);
                // Chunk exists, do check
                if (chunk->getBlock(Position{ newCheckX, newCheckY, newCheckZ }) != Block(0)) continue;
            }

            addFace((Block::Face)i, offset, textureIndex);
        }

    };
    
    for (int i=0; i<Chunk::ChunkBlockCount; i++) {
        glm::vec3 p = ReverseChunkIndexFormula(Chunk::ChunkX, Chunk::ChunkZ, i);

        addBlock(p, 0);
    }

    glBindBuffer(GL_ARRAY_BUFFER, vertexIndexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLuint) * vertexIndex.size(), vertexIndex.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vertexPositionBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * offsets.size(), offsets.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vertexTextureBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLuint) * textureIndexs.size(), textureIndexs.data(), GL_STATIC_DRAW);

    meshUpToDate = true;
    lastVertexCount = vertexIndex.size();
}

void DrawableChunk::renderChunk(std::unique_ptr<WorldRenderInfo>& renderInfo, glm::mat4 MVP) {
    auto& shader = renderInfo->getTerrainRenderingShader();
    const auto& uniforms = renderInfo->getTerrainRenderingUniforms();
    
    shader->use();
    glUniformMatrix4fv(uniforms.mvp, 1, GL_FALSE, &MVP[0][0]);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, renderInfo->getTerrainTexture());
    glUniform1i(uniforms.sampler, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vertexIndexBuffer);
    glEnableVertexAttribArray(0);
	glVertexAttribIPointer(0, 1, GL_UNSIGNED_INT, 0, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, vertexPositionBuffer);
    glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, vertexTextureBuffer);
    glEnableVertexAttribArray(2);
	glVertexAttribIPointer(2, 1, GL_UNSIGNED_INT, 0, nullptr);

    glDrawArrays(GL_TRIANGLES, 0, lastVertexCount);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}