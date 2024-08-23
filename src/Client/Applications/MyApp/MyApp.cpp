#include "MyApp.hpp"

#include <functional>

MyApp::MyApp() {}
MyApp::~MyApp() {}

void MyApp::start() {
    renderThread = std::thread(std::bind(&MyApp::runGame, this));
}

void MyApp::join() {
    renderThread.join();
}