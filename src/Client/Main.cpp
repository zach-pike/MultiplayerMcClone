// #include "Applications/MyApp/MyApp.hpp"

#include "BaseGameClient/BaseGameClient.hpp"

int main() {
    asio::ip::tcp::endpoint endpoint(asio::ip::address::from_string("127.0.0.1"), 1500);
    BaseGameClient client;

    client.connect(endpoint);

    while(!client.isMessageAvailable()) {
        std::cout << "Waiting on message... ";
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(500ms);
    }

    Message m = client.getMessage();
    std::string s = std::string(m.data.begin(), m.data.end());

    std::cout << "ID: " << m.id << " " << s << '\n';

    std::cout << "Sending reply!\n";
    client.sendMessage(Message(101, "Test working!"));

    client.disconnect();

    return 0; 
}