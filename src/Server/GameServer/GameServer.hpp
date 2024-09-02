#pragma once

#include "BaseGameServer/BaseGameServer.hpp"
#include "Player/Player.hpp"
#include "World/World.hpp"

class GameServer : private BaseGameServer {
private:
    std::atomic_bool messageHandlerThreadRunning;
    std::atomic_bool gameProcessingThreadRunning;

    std::thread      messageHandlerThread;
    std::thread      gameProcessingThread;

    std::vector<std::shared_ptr<Player>> players;
    std::mutex                           playersLock;

    std::vector<std::shared_ptr<Client>> connectingPlayers;
    std::mutex                           connectingPlayersLock;

    World gameWorld;
protected:
    void onConnect(std::shared_ptr<Client> client);
public:
    GameServer(std::uint16_t port);
    ~GameServer();

    void start();
    void stop();
};