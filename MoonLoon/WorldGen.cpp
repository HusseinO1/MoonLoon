#include "WorldGen.h"
#include "PerlinGeneration.h"
#include "CustomMath.h"
#include "Spike.h"

int WorldGen::baseHeight = 40;
int WorldGen::surfaceHeights[World::TILES_W];

namespace
{
	constexpr double FREQUENCY = 0.05;
	constexpr double AMPLITUDE = 10.0;
	constexpr double SEED_OFFSET = 1000.37;
	constexpr double SAMPLE_Y = 0.5;

	constexpr int MIN_SPIKES = 17;
	constexpr int MAX_SPIKES = 25;
	constexpr int SPAWN_COLUMN = 16;
	constexpr int SPAWN_CLEARANCE = 5;   
	constexpr int EDGE_MARGIN = 3; 

	unsigned rngState = static_cast<unsigned>(SEED_OFFSET * 1000.0) | 1u;

	unsigned NextRandom()
	{
		rngState ^= rngState << 13;
		rngState ^= rngState >> 17;
		rngState ^= rngState << 5;
		return rngState;
	}
}

void WorldGen::GenerateWorld(World& world)
{
	const int minSurface = 1;
	const int maxSurface = World::TILES_H - DIRT_DEPTH - 2;

	for (int w = 0; w < World::TILES_W; ++w)
	{
		double sampleX = w * FREQUENCY + SEED_OFFSET;
		double noise = PerlinGeneration::PerlinNoise(sampleX, SAMPLE_Y) * 1.4;

		int columnSurface = static_cast<int>(
			CustomMath::Floor(baseHeight + noise * AMPLITUDE + 0.5));

		if (columnSurface < minSurface) columnSurface = minSurface;
		if (columnSurface > maxSurface) columnSurface = maxSurface;

		surfaceHeights[w] = columnSurface;

		for (int h = 0; h < World::TILES_H; ++h)
		{
			if (h < columnSurface)
				continue;
			else if (h == columnSurface)
				world.SetTile(w, h, TileType::Grass);
			else if (h - columnSurface <= DIRT_DEPTH)
				world.SetTile(w, h, TileType::Dirt);
			else
				world.SetTile(w, h, TileType::Stone);
		}
	}

	const int range = MAX_SPIKES - MIN_SPIKES + 1;
	const int count = MIN_SPIKES + static_cast<int>(NextRandom() % static_cast<unsigned>(range));

	const int usableWidth = World::TILES_W - 2 * EDGE_MARGIN;
	const int segmentWidth = usableWidth / count;

	if (segmentWidth >= 1)
	{
		for (int i = 0; i < count; ++i)
		{
			const int segStart = EDGE_MARGIN + i * segmentWidth;

			int col = -1;
			for (int attempt = 0; attempt < 10; ++attempt)
			{
				const int candidate = segStart + static_cast<int>(NextRandom() % static_cast<unsigned>(segmentWidth));
				const int dist = candidate > SPAWN_COLUMN ? candidate - SPAWN_COLUMN : SPAWN_COLUMN - candidate;
				if (dist >= SPAWN_CLEARANCE)
				{
					col = candidate;
					break;
				}
			}
			if (col < 0) continue;  

			Spike::PlantSpike(world, col, surfaceHeights[col] - 1);
		}
	}
}

int WorldGen::GetSurfaceHeight(int column)
{
	if (column < 0) column = 0;
	if (column >= World::TILES_W) column = World::TILES_W - 1;
	return surfaceHeights[column];
}