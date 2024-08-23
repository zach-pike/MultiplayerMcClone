#version 410 core

out vec3 color;

in vec3 finalColor;

void main() {
    color = finalColor;
}