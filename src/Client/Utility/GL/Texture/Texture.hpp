#pragma once

#include <glad/glad.h>

#include <string>

class Texture {
private:
    GLuint textureId;
public:
    Texture();
    ~Texture();

    void loadFromFilePath(std::string path);

    void uploadTexture2DFromBuffer(const void* data, std::size_t width, std::size_t height, GLenum format, GLenum type);

    GLuint getTextureId();
};