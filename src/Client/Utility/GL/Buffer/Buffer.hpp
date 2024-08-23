#pragma once

#include "glad/glad.h"

#include <span>


template <typename T>
class Buffer {
private:
    GLuint buffer;
    bool   ready = false;

    std::size_t lastBufferedCount = 0;
public:
    Buffer() {
        glGenBuffers(1, &buffer);
    }
    
    ~Buffer() {
        glDeleteBuffers(1, &buffer);
    }


    void bufferData(std::span<T> data) {
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
	    glBufferData(GL_ARRAY_BUFFER, sizeof(T)*data.size(), data.data(), GL_STATIC_DRAW);

        lastBufferedCount = data.size();
    }

    GLuint getBufferId() {
        return buffer;
    }

    std::size_t size() const {
        return lastBufferedCount;
    }
};