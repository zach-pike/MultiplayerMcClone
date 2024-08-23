#include "Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 _pos, float _pitch, float _yaw):
    pos(_pos),
    pitch(_pitch),
    yaw(_yaw) {}

Camera::~Camera() {}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(pos, pos + getLookingVector(), glm::vec3(0, 1, 0));
}

glm::vec3 Camera::getLookingVector() const {
    return glm::vec3(cos(pitch)*cos(yaw), sin(pitch), sin(yaw) * cos(pitch));
}