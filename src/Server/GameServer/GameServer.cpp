#include "GameServer.hpp"

#include "Common/MessagePreprocessor/MessagePreprocessor.hpp"

#include "Common/GameMessages/Player/SCPlayerData/SCPlayerData.hpp"
#include "Common/GameMessages/Player/CSUpdatePlayerInfo/CSUpdatePlayerInfo.hpp"

#include "Common/GameMessages/Connection/Connect/SCConnectInfo/SCConnectInfo.hpp"
#include "Common/GameMessages/Connection/Connect/CSConnectInfo/CSConnectInfo.hpp"

#include "Common/MessagePreprocessor/MessagePreprocessor.hpp"

#include <thread>

void GameServer::onConnect(std::shared_ptr<Client> client) {
    // Send initial setup info
    auto scConnect = std::make_shared<MessageTypes::SCConnectInfo>();
    client->sendMessage(processMessage(scConnect));

    std::scoped_lock lock(connectingPlayersLock);
    connectingPlayers.push_back(client);
}

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

            // Lock connecting player list
            connectingPlayersLock.lock();

            for (int i=0; i<connectingPlayers.size(); i++) {
                std::shared_ptr<Client>& c = connectingPlayers.at(i);

                // Check for any messages
                if (c->isMessageAvailable()) {
                    Message m = c->getMessage();

                    assert((m.id == (MessageIDType)MessageTypes::GameMessageType::CS_Connect_Info));
                    auto packet = std::make_shared<MessageTypes::CSConnectInfo>();
                    packet->deserialize(m.data);

                    auto player = std::make_shared<Player>(c);
                    player->setUsername(packet->getUsername());

                    connectingPlayers.erase(connectingPlayers.begin() + i);
                    i = i - 1;

                    playersLock.lock();
                    players.push_back(player);
                    playersLock.unlock();
                }
            }

            // Unlock connecting player list
            connectingPlayersLock.unlock();

            std::vector<std::shared_ptr<Client>> disconnectedClients;

            for (int i=0; i < clients.size(); i++) {
                auto& conn = clients[i];

                if (!conn->isConnected()) {
                    // Fix disconnection
                    disconnectedClients.push_back(conn);
                }
            }

            // Lock players list
            playersLock.lock();

            for (int i=0; i<players.size(); i++) {
                auto& player = players.at(i);
                auto client = player->getClient();

                if (std::find(disconnectedClients.begin(), disconnectedClients.end(), client) != disconnectedClients.end()) {
                    // Client has disconnected
                    // Clean up client
                    players.erase(players.begin() + i);
                    
                    auto& clients = BaseGameServer::getClients();
                    auto clientIter = std::find(clients.begin(), clients.end(), client);

                    clients.erase(clientIter);

                    i = i - 1;
                    continue;
                }

                // Check for messages
                while(client->isMessageAvailable()) {
                    Message m = client->getMessage();
                    MessageTypes::GameMessageType msgType = (MessageTypes::GameMessageType)m.id;

                    switch (msgType) {
                        case MessageTypes::GameMessageType::CS_Update_Player_Info: {
                            auto msg = std::make_shared<MessageTypes::CSUpdatePlayerInfo>();
                            msg->deserialize(m.data);

                            player->setPosition(msg->getPosition());
                        } break;
                    }
                }
            }

            // Unlock playerlist
            playersLock.unlock();

            using namespace std::chrono_literals;
            std::this_thread::sleep_for(10ms);
        }
    });

    gameProcessingThread = std::thread([&]() {
        while(gameProcessingThreadRunning) {
            auto start = std::chrono::high_resolution_clock::now();

            // Processing code here
            // Send player info
            std::vector<Vec3fPosition> positions;
            std::vector<std::string> usernames;

            playersLock.lock();
            for (auto& player : players) {
                auto pos = player->getPosition();
                auto username = player->getUsername();

                positions.push_back(pos);
                usernames.push_back(username);
            }
            playersLock.unlock();

            auto packet = std::make_shared<MessageTypes::SCPlayerData>();
            packet->setPlayerUsernames(usernames);
            packet->setPlayerPositions(positions);

            playersLock.lock();
            for (auto& p : players) {
                p->getClient()->sendMessage(processMessage(packet));
            }
            playersLock.unlock();

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