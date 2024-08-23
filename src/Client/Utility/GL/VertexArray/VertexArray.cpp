#include "VertexArray.hpp"

VertexArray::VertexArray() {
    glGenVertexArrays(1, &vertexArray);
}

VertexArray::~VertexArray() {
    glDeleteVertexArrays(1, &vertexArray);
}

void VertexArray::bindVertexArray() {
    glBindVertexArray(vertexArray);
}