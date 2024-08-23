#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Utility/Camera/Camera.hpp"
#include <memory>

class FreecamController {
private:
    double lastMouseX, lastMouseY;
    Camera camera;

    bool isMouseLockedButtonPressed = false;

    bool isFirstStep = true;

    void lockMouse(GLFWwindow* window);
    void unlockMouse(GLFWwindow* window);
public:
    bool isMouseLocked = true;
    FreecamController(glm::vec3 cameraPos);
    ~FreecamController();

    const Camera& getCamera() const;

    void step(GLFWwindow* window, double deltaTime);
};