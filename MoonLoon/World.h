#pragma once
#include "Common.h"

enum class TileType : uchar
{
	Air = 0,
	Grass = 1,
	Dirt = 2,
	Stone = 3
};

struct Tile {
	TileType type;
	bool spiked = false;
};

struct Chunk {
	int cx;
	int cy;
	Tile tiles[CHUNK_SIZE][CHUNK_SIZE]; //[y][x]

	Chunk()
	{
		cx = 0;
		cy = 0;
		for (int y = 0; y < CHUNK_SIZE; ++y) {
			for (int x = 0; x < CHUNK_SIZE; ++x) {
				tiles[y][x].type = TileType::Air;
			}
		}
	}

	Tile GetTile(int x, int y) const {
		if (x < 0 || x >= CHUNK_SIZE || y < 0 || y >= CHUNK_SIZE) return Tile{ TileType::Air };
		return tiles[y][x];
	}

	void SetTile(int x, int y, TileType type) {
		if (x < 0 || x >= CHUNK_SIZE || y < 0 || y >= CHUNK_SIZE) return;
		tiles[y][x].type = type;
	}
};

class World
{
public:
	static constexpr int WORLD_WIDTH = 16; 
	static constexpr int WORLD_HEIGHT = 8;   
	static constexpr int TILES_W = WORLD_WIDTH * CHUNK_SIZE;
	static constexpr int TILES_H = WORLD_HEIGHT * CHUNK_SIZE;

	Chunk world[WORLD_HEIGHT][WORLD_WIDTH];

	World()
	{
		for (int cy = 0; cy < WORLD_HEIGHT; ++cy) {
			for (int cx = 0; cx < WORLD_WIDTH; ++cx) {
				world[cy][cx].cx = cx;
				world[cy][cx].cy = cy;
			}
		}
	}

	Chunk* GetChunk(int cx, int cy) {
		if (cx < 0 || cx >= WORLD_WIDTH || cy < 0 || cy >= WORLD_HEIGHT) return nullptr;
		return &world[cy][cx];
	}

	Tile GetTile(int wx, int wy) {
		Chunk* chunk = GetChunk(wx >> CHUNK_SHIFT, wy >> CHUNK_SHIFT);
		if (!chunk) return Tile{ TileType::Air };
		return chunk->GetTile(wx & (CHUNK_SIZE - 1), wy & (CHUNK_SIZE - 1));
	}

	void SetTile(int wx, int wy, TileType type) {
		Chunk* chunk = GetChunk(wx >> CHUNK_SHIFT, wy >> CHUNK_SHIFT);
		if (!chunk) return;
		chunk->SetTile(wx & (CHUNK_SIZE - 1), wy & (CHUNK_SIZE - 1), type);
	}
};