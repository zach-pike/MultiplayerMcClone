#include "Game/Game.hpp"
#include <memory>

int main() {
    auto app = std::make_shared<Game>();

    app->start();
    app->join();


    return 0; 
}