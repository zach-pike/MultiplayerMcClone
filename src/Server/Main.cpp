#include "BaseGameServer/BaseGameServer.hpp"

int main() {
    BaseGameServer server(1500);
    server.start();

    using namespace std::chrono_literals;
    std::this_thread::sleep_for(5s);

    server.broadcast(Message(101, "Hello world!"));

    std::this_thread::sleep_for(5s);

    // Check for a reply
    for (auto& conn : server.getClients()) {
        while(!conn->isMessageAvailable());

        // Get message
        Message m = conn->getMessage();

        std::string s(m.data.begin(), m.data.end());

        std::cout << "Reply!: ID: " << m.id << " " << s << '\n';
    }

    server.stop();
}