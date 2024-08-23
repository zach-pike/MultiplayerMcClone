#include "asio.hpp"

#include <vector>
#include <cstdint>
#include <iostream>
#include <deque>

#include "Common/MessageTypes.hpp"

class BaseGameClient {
private:
    MessageHeader _incHeader;
    std::vector<std::uint8_t> _incPayload;

    std::mutex          inMessageQueueLock;
    std::deque<Message> inMessageQueue;

    void readHeader();
    void readPayload();

    asio::io_context      ctx;
    std::thread           ctxThread;
    asio::ip::tcp::socket sock;
public:
    BaseGameClient();
    ~BaseGameClient();

    void connect(asio::ip::tcp::endpoint ep);
    void disconnect();

    void sendMessage(const Message& msg);
    bool isMessageAvailable();
    Message getMessage();
};