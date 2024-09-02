#pragma once

#include "Client/Client.hpp"

#include "asio.hpp"

#include <vector>
#include <cstdint>
#include <iostream>

class BaseGameServer {
protected:
    virtual void onConnect(std::shared_ptr<Client> client) = 0;
private:
    asio::io_context        ctx;
    std::thread             networkThread;

    asio::ip::tcp::acceptor acceptor;

    std::vector<std::shared_ptr<Client>> connections;

    void acceptConnection();
public:
    BaseGameServer(std::uint16_t port);

    ~BaseGameServer();

    void start();
    void stop();

    std::vector<std::shared_ptr<Client>>& getClients();

    void broadcast(const Message& msg);
};