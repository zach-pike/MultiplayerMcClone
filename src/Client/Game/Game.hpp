#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "App/App.hpp"

#include <thread>

#include "Utility/BaseGameClient/BaseGameClient.hpp"

class Game : public App {
private:
    // Thread state do not modify pls thx
    GLFWwindow* window;

    std::thread renderThread;

    BaseGameClient client;

    void init();
    void loop();
    void cleanup();

    void runGame();
public:
    Game();
    ~Game();

    void start();
    void join();
};