#pragma once

#include <cstdint>
#include <vector>

using MessageIDType = std::uint32_t;

struct Message {
    MessageIDType id;
    std::vector<std::uint8_t> data;

    Message() {}

    Message(MessageIDType _id, std::string str) {
        id = _id;
        data = std::vector<std::uint8_t>(str.begin(), str.end());
    }
};

struct MessageHeader {
    MessageIDType id;
    std::uint32_t dataSize;

    MessageHeader() {}

    MessageHeader(const Message& msg) {
        id = msg.id;
        dataSize = msg.data.size();
    }
};