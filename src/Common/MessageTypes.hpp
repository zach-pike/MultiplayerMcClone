#pragma once

#include <cstdint>
#include <vector>
#include <string>

using MessageIDType = std::uint32_t;

struct Message {
    MessageIDType id;
    std::vector<std::uint8_t> data;

    Message() {}
    Message(MessageIDType _id) {
        id = _id;
    }
    Message(MessageIDType _id, const std::vector<std::uint8_t>& _data) {
        id = _id;
        data = _data;
    }
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

struct Vec3fPosition {
    float x, y, z;
};

struct Vec3iPosition {
    std::int32_t x, y, z;
};