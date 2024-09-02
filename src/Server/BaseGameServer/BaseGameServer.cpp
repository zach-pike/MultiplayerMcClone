#include "BaseGameServer.hpp"

void BaseGameServer::acceptConnection() {
    acceptor.async_accept([&](asio::error_code ec, asio::ip::tcp::socket sock) {
        if (ec) {
            // Error
            std::cout << "Acceptor Error!\n";

            acceptConnection();
        }

        auto conn = std::make_shared<Client>(std::move(sock));

        // No error
        connections.push_back(conn);

        onConnect(conn);

        acceptConnection();
    });
}

BaseGameServer::BaseGameServer(std::uint16_t port):
    acceptor(ctx, asio::ip::tcp::endpoint(asio::ip::address::from_string("0.0.0.0"), port)) {}

BaseGameServer::~BaseGameServer() {}

void BaseGameServer::start() {
    acceptConnection();

    networkThread = std::thread([&]() { ctx.run(); });
}

void BaseGameServer::stop() {
    ctx.stop();
    networkThread.join();
}

void BaseGameServer::broadcast(const Message& msg) {
    // Send message to all clients
    for (auto& client : connections) {
        client->sendMessage(msg);
    }
}

std::vector<std::shared_ptr<Client>>& BaseGameServer::getClients() {
    return connections;
}