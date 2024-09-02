#include "Player.hpp"

Player::Player(std::shared_ptr<Client> _client):
    connection(_client) {}
    
Player::~Player() {}

Vec3fPosition Player::getPosition() const {
    std::scoped_lock lock(worldPositionLock);

    return worldPosition;
}

void Player::setPosition(Vec3fPosition p) {
    std::scoped_lock lock(worldPositionLock);

    worldPosition = p;   
}

std::string Player::getUsername() const {
    std::scoped_lock lock(usernameLock);

    return username;
}

void Player::setUsername(std::string s) {
    std::scoped_lock lock(usernameLock);

    username = s;
}

std::shared_ptr<Client> Player::getClient() {
    return connection;
}