#include "Texture.hpp"

#include <stb_image.h>

#include <cstdint>

Texture::Texture() {
    glGenTextures(1, &textureId);
}
Texture::~Texture() {
    glDeleteTextures(1, &textureId);
}

void Texture::loadFromFilePath(std::string path) {
    int width, height, channels;
	std::uint8_t* img = stbi_load(path.c_str(), &width, &height, nullptr, STBI_rgb_alpha);

    uploadTexture2DFromBuffer(img, width, height, GL_RGBA, GL_UNSIGNED_BYTE);

    stbi_image_free(img);
}

void Texture::uploadTexture2DFromBuffer(const void* data, std::size_t width, std::size_t height, GLenum format, GLenum type) {
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, type, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
}

GLuint Texture::getTextureId() {
    return textureId;
}