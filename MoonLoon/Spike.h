#pragma once
#include "World.h"
#include "Common.h"
class Spike
{
public:
	Spike();
	static void PlantSpike(World& world, int tileX, int tileY);
	static void DrawSpike(int tileX, int tileY, const Camera& camera);
};

