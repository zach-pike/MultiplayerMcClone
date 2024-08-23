#include "FreecamController.hpp"

#include <glm/gtc/matrix_transform.hpp>

FreecamController::FreecamController(glm::vec3 pos):
    camera(pos, M_PI, 0)
{}

FreecamController::~FreecamController() {}

void FreecamController::lockMouse(GLFWwindow* window) {
    isMouseLocked = true;
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void FreecamController::unlockMouse(GLFWwindow* window) {
    isMouseLocked = false;
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

const Camera& FreecamController::getCamera() const {
    return camera;
}

void FreecamController::step(GLFWwindow* window, double deltaTime) {
    const float mouseSens = .35f;
    const float moveSpeed = 25.f;

    if (isFirstStep) {
        if (isMouseLocked) glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        glfwSetCursorPos(window, 0, 0);
        isFirstStep = false;
    }

    // Camera lock toggle key
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        if (isMouseLockedButtonPressed == false) {
            isMouseLockedButtonPressed = true;

            if (isMouseLocked) {
                unlockMouse(window);
            } else {
                // Stop jumping when relocking mouse
                glfwSetCursorPos(window, 0, 0);
                lastMouseX = 0;
                lastMouseY = 0;
                
                lockMouse(window);
            }
        }
    } else {
        isMouseLockedButtonPressed = false;
    }

    if (isMouseLocked) {
        double mousePosX, mousePosY;
        glfwGetCursorPos(window, &mousePosX, &mousePosY);

        double mouseDeltaX = mousePosX - lastMouseX;
        double mouseDeltaY = mousePosY - lastMouseY;

        mouseDeltaX *= deltaTime;
        mouseDeltaY *= deltaTime;

        lastMouseX = mousePosX;
        lastMouseY = mousePosY;

        camera.yaw += mouseDeltaX * mouseSens;

        // Lock the camera to almost all the way down and almost all the way up
        camera.pitch = std::min(1.5f*M_PI - .01, std::max((double)M_PI_2 + .01, camera.pitch + mouseDeltaY * mouseSens));
    }


    // Now player movement
    auto looking = getCamera().getLookingVector();

    // Sprint key
    const float moveSpeedMultiplier = glfwGetKey(window, GLFW_KEY_LEFT_ALT) != GLFW_PRESS ? 1.f : 3.5f;

    // Forward
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.pos += looking * moveSpeed * moveSpeedMultiplier * glm::vec3(deltaTime);
    }
    // Back
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.pos -= looking * moveSpeed * moveSpeedMultiplier * glm::vec3(deltaTime);
    }
    // Left
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.pos += glm::normalize(glm::vec3(looking.z, 0, -looking.x)) * moveSpeed * moveSpeedMultiplier * glm::vec3(deltaTime);
    }
    // Right
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.pos -= glm::normalize(glm::vec3(looking.z, 0, -looking.x)) * moveSpeed * moveSpeedMultiplier * glm::vec3(deltaTime);
    }
    // Up
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        camera.pos += glm::vec3(0, 1, 0) * moveSpeed * moveSpeedMultiplier * glm::vec3(deltaTime);
    }
    // Down
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        camera.pos -= glm::vec3(0, 1, 0) * moveSpeed * moveSpeedMultiplier * glm::vec3(deltaTime);
    }
}