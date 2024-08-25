#include "GameServer.hpp"

#include <thread>

GameServer::GameServer(std::uint16_t port):
    BaseGameServer(port)
{
    gameWorld.generateWorld();
}

GameServer::~GameServer() {

}

void GameServer::start() {
    BaseGameServer::start();

    messageHandlerThreadRunning = true;
    gameProcessingThreadRunning = true;

    messageHandlerThread = std::thread([&]() {
        while(messageHandlerThreadRunning) {
            auto& clients = BaseGameServer::getClients();

            for (int i=0; i < clients.size(); i++) {
                auto& conn = clients[i];

                if (!conn->isConnected()) {
                    // Fix disconnection

                    clients.erase(clients.begin() + i);

                    i = -1;
                    continue;

                } else if (conn->isMessageAvailable()) {
                    Message m = conn->getMessage();

                    switch (m.id) {
                        case 1337: {
                            // Convert world to data
                            std::vector<std::uint8_t> d = gameWorld.serialize();

                            // Send vertex data
                            conn->sendMessage(Message(1337, d));
                        } break;
                    }
                }
            }

            using namespace std::chrono_literals;
            std::this_thread::sleep_for(10ms);
        }
    });

    gameProcessingThread = std::thread([&]() {
        while(gameProcessingThreadRunning) {
            auto start = std::chrono::high_resolution_clock::now();

            // Processing code here
            // WIP...

            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> elapsed = end - start;

            // Sleep for the remainder of the time to make it 50ms per loop
            std::this_thread::sleep_for(std::chrono::milliseconds(50) - elapsed);
        }
    });
}

void GameServer::stop() {
    BaseGameServer::stop();

    messageHandlerThreadRunning = false;
    gameProcessingThreadRunning = false;

    messageHandlerThread.join();
    gameProcessingThread.join();
}