#include "Client.hpp"

void Client::readHeader() {
    asio::async_read(sock, asio::buffer((std::uint8_t*)&_incHeader, sizeof(MessageHeader)), [&](asio::error_code ec, std::size_t len) {
        readPayload();
    });
}

void Client::readPayload() {
    // Allocate space for body
    _incPayload.resize(_incHeader.dataSize);

    asio::async_read(sock, asio::buffer(_incPayload.data(), _incPayload.size()), [&](asio::error_code ec, std::size_t len) {
        Message m;
        m.id = _incHeader.id;
        m.data = _incPayload;

        inMessagesQueueLock.lock();
        inMessagesQueue.push_back(m);
        inMessagesQueueLock.unlock();

        readHeader();
    });
}

Client::Client(asio::ip::tcp::socket _sock):
    sock(std::move(_sock)) 
{
    // Start cycle
    readHeader();
}

Client::~Client() {}

void Client::sendMessage(const Message& msg) {
    MessageHeader hdr(msg);

    sock.write_some(asio::buffer(&hdr, sizeof(hdr)));
    sock.write_some(asio::buffer(msg.data.data(), msg.data.size()));
}

bool Client::isMessageAvailable() {
    inMessagesQueueLock.lock();
    bool avail = inMessagesQueue.size() > 0;
    inMessagesQueueLock.unlock();
    return avail;
}

Message Client::getMessage() {
    inMessagesQueueLock.lock();

    Message m = inMessagesQueue.front();
    inMessagesQueue.pop_front();

    inMessagesQueueLock.unlock();
    return m;
}