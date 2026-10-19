#include "Spike.h"
#include "WinBuffer.h"
#include "World.h"

namespace
{
	void DrawLine(int x0, int y0, int x1, int y1, int r, int g, int b)
	{
		int dx = x1 > x0 ? x1 - x0 : x0 - x1;
		int dy = y1 > y0 ? y1 - y0 : y0 - y1;
		int sx = x0 < x1 ? 1 : -1;
		int sy = y0 < y1 ? 1 : -1;
		int err = dx - dy;

		while(true)
		{
			WinBuffer::FillRect(x0, y0, 1, 1, r, g, b);
			if(x0 == x1 && y0 == y1) break;

			int e2 = 2 * err;
			if(e2 > -dy) { err -= dy; x0 += sx; }
			if(e2 < dx) { err += dx; y0 += sy; }
		}
	}
}

Spike::Spike()
{}

void Spike::PlantSpike(World& world, int tileX, int tileY)
{
	Chunk* chunk = world.GetChunk(tileX >> CHUNK_SHIFT, tileY >> CHUNK_SHIFT);
	if (!chunk) return;

	chunk->tiles[tileY & (CHUNK_SIZE - 1)][tileX & (CHUNK_SIZE - 1)].spiked = true;
}

void Spike::DrawSpike(int tileX, int tileY, const Camera& camera)
{
	const int camX = static_cast<int>(camera.x);
	const int camY = static_cast<int>(camera.y);

	
	const int leftX = tileX * TILE_SIZE - camX;
	const int leftY = (tileY + 1) * TILE_SIZE - camY;
	const int topX = tileX * TILE_SIZE + TILE_SIZE / 2 - camX;
	const int topY = tileY * TILE_SIZE - camY;
	const int rightX = (tileX + 1) * TILE_SIZE - camX;
	const int rightY = (tileY + 1) * TILE_SIZE - camY;

	DrawLine(leftX, leftY, topX, topY, 255, 60, 60);
	DrawLine(topX, topY, rightX, rightY, 255, 60, 60);
	DrawLine(rightX, rightY, leftX, leftY, 255, 60, 60);
}