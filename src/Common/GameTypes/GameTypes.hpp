#pragma once

struct ChunkCoordinates {
    int x, y, z;

    bool operator<(const ChunkCoordinates& other) const;
};
struct Position { int x, y, z; };