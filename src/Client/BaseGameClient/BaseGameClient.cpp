#include "BaseGameClient.hpp"

void BaseGameClient::readHeader() {
    asio::async_read(sock, asio::buffer((std::uint8_t*)&_incHeader, sizeof(MessageHeader)), [&](asio::error_code ec, std::size_t len) {
        if (ec) {
            socketOpen = false;
            return;
        }

        readPayload();
    });
}

void BaseGameClient::readPayload() {
    // Allocate space for body
    _incPayload.resize(_incHeader.dataSize);

    asio::async_read(sock, asio::buffer(_incPayload.data(), _incPayload.size()), [&](asio::error_code ec, std::size_t len) {
        if (ec) {
            socketOpen = false;
            return;
        }

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
    socketOpen = true;
    readHeader();
    ctxThread = std::thread([&]() { ctx.run(); });
}

void BaseGameClient::disconnect() {
    sock.close();
    ctx.stop();
    ctxThread.join();

    socketOpen = false;
}

bool BaseGameClient::isConnected() {
    return socketOpen;
}

void BaseGameClient::sendMessage(const Message& msg) {
    if (!socketOpen) throw std::runtime_error("Connection closed");
    
    MessageHeader hdr(msg);

    asio::error_code ec;
    sock.write_some(asio::buffer(&hdr, sizeof(hdr)), ec);
    if (ec) {
        socketOpen = false;
        return;
    }

    sock.write_some(asio::buffer(msg.data.data(), msg.data.size()), ec);
    if (ec) {
        socketOpen = false;
        return;
    }
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