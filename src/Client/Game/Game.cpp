#include "Game.hpp"

#include <functional>

Game::Game() {}
Game::~Game() {}

void Game::start() {
    renderThread = std::thread(std::bind(&Game::runGame, this));
}

void Game::join() {
    renderThread.join();
}