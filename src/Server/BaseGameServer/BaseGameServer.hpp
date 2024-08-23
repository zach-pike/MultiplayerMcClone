#pragma once

#include "Client/Client.hpp"

#include "asio.hpp"

#include <vector>
#include <cstdint>
#include <iostream>

class BaseGameServer {
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

    std::vector<std::shared_ptr<Client>> getClients();

    void broadcast(const Message& msg);
};