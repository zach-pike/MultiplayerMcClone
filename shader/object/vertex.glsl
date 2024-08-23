#version 410 core

layout(location=0) in vec3 vertexPosition;
layout(location=1) in vec3 colorValue;

uniform mat4 viewProjection;

out vec3 finalColor;

void main() {
    gl_Position = viewProjection * vec4(vertexPosition, 1);

    finalColor = colorValue;
}