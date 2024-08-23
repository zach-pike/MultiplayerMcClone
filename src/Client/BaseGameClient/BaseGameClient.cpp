#include "BaseGameClient.hpp"

void BaseGameClient::readHeader() {
    asio::async_read(sock, asio::buffer((std::uint8_t*)&_incHeader, sizeof(MessageHeader)), [&](asio::error_code ec, std::size_t len) {
        readPayload();
    });
}

void BaseGameClient::readPayload() {
    // Allocate space for body
    _incPayload.resize(_incHeader.dataSize);

    asio::async_read(sock, asio::buffer(_incPayload.data(), _incPayload.size()), [&](asio::error_code ec, std::size_t len) {
        Message m;
        m.id = _incHeader.id;
        m.data = _incPayload;

        inMessageQueueLock.lock();
        inMessageQueue.push_back(m);
        inMessageQueueLock.unlock();

        readHeader();
    });
}

BaseGameClient::BaseGameClient():
    sock(ctx) {}

BaseGameClient::~BaseGameClient() {}

void BaseGameClient::connect(asio::ip::tcp::endpoint ep) {
    sock.connect(ep);
    readHeader();
    ctxThread = std::thread([&]() { ctx.run(); });
}

void BaseGameClient::disconnect() {
    sock.close();
    ctx.stop();
    ctxThread.join();
}

void BaseGameClient::sendMessage(const Message& msg) {
    // Write header
    MessageHeader header(msg);
    sock.write_some(asio::buffer(&header, sizeof(header)));

    // Write body
    sock.write_some(asio::buffer(msg.data.data(), msg.data.size()));
}

bool BaseGameClient::isMessageAvailable() {
    inMessageQueueLock.lock();
    bool avail = inMessageQueue.size() > 0;
    inMessageQueueLock.unlock();
    return avail;
}

Message BaseGameClient::getMessage() {
    inMessageQueueLock.lock();

    Message m = inMessageQueue.front();
    inMessageQueue.pop_front();

    inMessageQueueLock.unlock();
    return m;
}