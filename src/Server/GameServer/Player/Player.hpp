#pragma once

#include "BaseGameServer/Client/Client.hpp"
#include "Common/MessageTypes.hpp"
#include "Common/GameMessages/GameMessageTypes.hpp"

#include <memory>
#include <mutex>
#include <string>

class Player {
private:
    std::shared_ptr<Client> connection;

    Vec3fPosition worldPosition;
    mutable std::mutex    worldPositionLock;

    std::string username;
    mutable std::mutex  usernameLock;
public:
    Player(std::shared_ptr<Client> client);
    ~Player();

    Vec3fPosition getPosition() const;
    void setPosition(Vec3fPosition p);

    std::string getUsername() const;
    void setUsername(std::string s);

    std::shared_ptr<Client> getClient();
};