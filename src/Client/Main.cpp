// #include "Applications/MyApp/MyApp.hpp"

#define _WIN32_WINNT 0x0A00
#define ASIO_SEPARATE_COMPILATION
#include "asio.hpp"

#include <vector>
#include <cstdint>
#include <iostream>
#include <deque>

std::vector<std::uint8_t> recvBuffer(1024*4);
void ReadData(asio::error_code ec, std::size_t length) {
    std::cout << "aaa " << length << '\n';
    std::string d(recvBuffer.begin(), recvBuffer.end());

    std::cout << d;
}

class BasicClient {
public:
    using MessageIDType = std::uint32_t;

    struct Message;

    struct MessageHeader {
        MessageIDType id;
        std::uint32_t dataSize;

        MessageHeader() {}

        MessageHeader(const Message& msg) {
            id = msg.id;
            dataSize = msg.data.size();
        }
    };

    struct Message {
        MessageIDType id;
        std::vector<std::uint8_t> data;
    };
private:
    MessageHeader incomingHdr;
    std::vector<std::uint8_t> incomingBody;

    std::mutex          inMessageQueueLock;
    std::deque<Message> inMessageQueue;

    void readHeader() {
        asio::async_read(sock, asio::buffer((std::uint8_t*)&incomingHdr, sizeof(MessageHeader)), [&](asio::error_code ec, std::size_t len) {
            readBody();
        });
    }

    void readBody() {
        // Allocate space for body
        incomingBody.resize(incomingHdr.dataSize);

        asio::async_read(sock, asio::buffer(incomingBody.data(), incomingBody.size()), [&](asio::error_code ec, std::size_t len) {
            Message m;
            m.id = incomingHdr.id;
            m.data = incomingBody;

            inMessageQueueLock.lock();
            inMessageQueue.push_back(m);
            inMessageQueueLock.unlock();

            readHeader();
        });
    }

    asio::ip::tcp::socket sock;
public:
    BasicClient(asio::io_context& ctx, asio::ip::tcp::endpoint ep):
        sock(ctx) {
            sock.connect(ep);

            readHeader();
        }

    ~BasicClient() {
        sock.close();
    }

    void sendMessage(const Message& msg) {
        // Write header
        MessageHeader header(msg);
        sock.write_some(asio::buffer(&header, sizeof(header)));

        // Write body
        sock.write_some(asio::buffer(msg.data.data(), msg.data.size()));
    }

    bool isMessageAvailable() {
        inMessageQueueLock.lock();
        bool avail = inMessageQueue.size() > 0;
        inMessageQueueLock.unlock();
        return avail;
    }

    Message getMessage() {
        inMessageQueueLock.lock();

        Message m = inMessageQueue.front();
        inMessageQueue.pop_front();

        inMessageQueueLock.unlock();
        return m;
    }
};

int main() {
    // auto game = std::make_unique<MyApp>();
    // game->start();
    // game->join();

    asio::io_context ctx;

    asio::io_context::work idleWork(ctx);
    std::thread thr = std::thread([&]() { ctx.run(); });

    asio::ip::tcp::endpoint endpoint(asio::ip::address::from_string("93.184.215.14"), 80);
    BasicClient client(ctx, endpoint);

    while(!client.isMessageAvailable()) {
        std::cout << "Waiting on message... ";
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(500ms);
    }

    

    return 0; 
}