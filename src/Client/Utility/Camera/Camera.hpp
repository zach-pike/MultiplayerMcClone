#pragma once

#include <glm/glm.hpp>

class Camera {
public:
    float pitch, yaw;
    glm::vec3 pos;

    Camera(glm::vec3 pos, float pitch, float yaw);
    ~Camera();

    glm::mat4 getViewMatrix() const;
    glm::vec3 getLookingVector() const;
};