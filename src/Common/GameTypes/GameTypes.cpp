#include "GameTypes.hpp"

bool ChunkCoordinates::operator<(const ChunkCoordinates& other) const {
    if (x != other.x) {
        return x < other.x;
    } else if (y != other.y) {
        return y < other.y;
    } else {
        return z < other.z;
    }
}