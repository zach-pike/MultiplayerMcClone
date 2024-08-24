#pragma once

#include "asio.hpp"

#include "Common/MessageTypes.hpp"

#include <deque>
#include <mutex>

class Client {
private:
    asio::ip::tcp::socket sock;

    std::mutex          inMessagesQueueLock;
    std::deque<Message> inMessagesQueue;

    std::atomic_bool socketOpen = true;

    MessageHeader _incHeader;
    std::vector<std::uint8_t> _incPayload;

    void readHeader();
    void readPayload();

public:
    Client(asio::ip::tcp::socket sock);
    ~Client();

    void sendMessage(const Message& msg);

    bool isConnected();

    bool isMessageAvailable();
    Message getMessage();
};