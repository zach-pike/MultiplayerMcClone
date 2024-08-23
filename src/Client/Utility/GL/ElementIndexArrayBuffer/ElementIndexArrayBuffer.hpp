#pragma once

#include <glad/glad.h>

#include <span>
#include <exception>

template <typename T, GLenum K>
class ElementIndexArrayBuffer {
private:
    GLuint buffer;
    std::uint64_t length = 0;
    bool bufferReady = false;

    void init() {
        glGenBuffers(1, &buffer);
        bufferReady = true;
    }
    void uninit() {
        glDeleteBuffers(1, &buffer);
        bufferReady = false;
    }
public:
    ElementIndexArrayBuffer() {}
    ~ElementIndexArrayBuffer() {
        if (bufferReady) uninit();
    }

    void bufferData(const std::span<T>& data) {
        if (!bufferReady) init();

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size()*sizeof(T), data.data(), GL_STATIC_DRAW);
        length = data.size();
    }

    void drawElements(GLenum primitave) {
        if (!bufferReady) throw std::runtime_error("Buffer not ready!");

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
        glDrawElements(primitave, length, K, (void*)0);
    }
};