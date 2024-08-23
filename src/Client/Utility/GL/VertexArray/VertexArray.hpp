#pragma once

#include "glad/glad.h"

class VertexArray {
private:
    GLuint vertexArray;
public:
    VertexArray(const VertexArray&) = delete; // non construction-copyable
    VertexArray& operator=(const VertexArray&) = delete; // non copyable

    VertexArray();
    ~VertexArray();

    void bindVertexArray();
};