#pragma once



using int64 = signed long long;
using uint64 = unsigned long long;
using uint = unsigned int;
using uchar = unsigned char;


struct vector2 {
    double x = 0.0;
    double y = 0.0;
};


struct Camera
{
    float x;
    float y;
};

struct Color { uchar r, g, b; };

constexpr Color TILE_COLORS[4] = {
    { 135, 190, 235 },        // Air (never drawn)
    { 170, 170, 175 },    // Grass
    { 120, 120, 125 },    // Dirt
    { 80, 80, 85 },  // Stone
};

template<typename T>
inline void Free(T*& ptr)
{
    delete ptr;
    ptr = nullptr;
}

template<typename T>
inline void FreeArray(T*& ptr)
{
    delete[] ptr;
    ptr = nullptr;
}

constexpr double deltaTime = 0.0166666666666667; //1/60

inline double totalTime = 0.0;

constexpr int TILE_SIZE = 64;

constexpr int CHUNK_SIZE = 16;

constexpr int CHUNK_SHIFT = 4;

constexpr int DIRT_DEPTH = 4;

static_assert((1 << CHUNK_SHIFT) == CHUNK_SIZE, "Chunk size mismatch");
