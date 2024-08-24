#pragma once

#include "BaseGameServer/BaseGameServer.hpp"

class GameServer : private BaseGameServer {
private:
    std::atomic_bool messageHandlerThreadRunning;
    std::atomic_bool gameProcessingThreadRunning;

    std::thread      messageHandlerThread;
    std::thread      gameProcessingThread;

public:
    GameServer(std::uint16_t port);
    ~GameServer();

    void start();
    void stop();
};