#include "WorldRenderInfo.hpp"

#include <cstdint>

#include "Utility/ShaderLoaders/VertexFragment/VertexFragment.hpp"

#include <stb_image.h>

void WorldRenderInfo::loadShaders() {
    terrainRenderShader = loadVertexFragmentShader("./shader/block/");

    terrainRenderShader->use();
    terrainRenderUniforms.mvp = terrainRenderShader->getUniformLocation("MVP");
    terrainRenderUniforms.sampler = terrainRenderShader->getUniformLocation("samp");
    terrainRenderShader->unbind();
}

void WorldRenderInfo::unloadShaders() {
    terrainRenderShader.reset();
}

void WorldRenderInfo::loadTextures() {
    glGenTextures(1, &terrainTexture);
    glBindTexture(GL_TEXTURE_2D_ARRAY, terrainTexture);

    const int numberTextures = 3;
    int sizeX = 16;
    int sizeY = 16;

    glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, sizeX, sizeY, numberTextures);
    const char* images[numberTextures] = { "resources\\dirt.png", "resources\\grass_side.png", "resources\\grass_top.png" };

    std::vector<std::uint8_t> imgdata;
    for (int i=0; i<numberTextures; i++) {
        int w, h;
        void* d = stbi_load(images[i], &w, &h, nullptr, 4);

        imgdata.insert(imgdata.end(), (std::uint8_t*)d, (std::uint8_t*)d + sizeX*sizeY*4);

        stbi_image_free(d);
    }
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, sizeX, sizeY, numberTextures, GL_RGBA, GL_UNSIGNED_BYTE, imgdata.data());

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void WorldRenderInfo::unloadTextures() {
    glDeleteTextures(1, &terrainTexture);
}

WorldRenderInfo::WorldRenderInfo() {
    loadShaders();
    loadTextures();
}

WorldRenderInfo::~WorldRenderInfo() {
    unloadShaders();
    unloadTextures();
}

void WorldRenderInfo::reloadShaders() {
    unloadShaders();
    loadShaders();
}

void WorldRenderInfo::reloadTextures() {
    unloadTextures();
    loadTextures();
}

std::unique_ptr<ShaderProgram>& WorldRenderInfo::getTerrainRenderingShader() {
    return terrainRenderShader;
}

const WorldRenderInfo::TerrainRenderingUniforms& WorldRenderInfo::getTerrainRenderingUniforms() const {
    return terrainRenderUniforms;
}

GLuint WorldRenderInfo::getTerrainTexture() {
    return terrainTexture;
}