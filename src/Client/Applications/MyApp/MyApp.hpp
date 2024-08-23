#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "App/App.hpp"

#include <thread>

class MyApp : public App {
private:
    // Thread state do not modify pls thx
    GLFWwindow* window;

    std::thread renderThread;

    void init();
    void loop();
    void cleanup();

    void runGame();
public:
    MyApp();
    ~MyApp();

    void start();
    void join();
};