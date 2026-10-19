#include "World.h"

class WorldGen
{
public:
	static void GenerateWorld(World& world);
	static int GetSurfaceHeight(int column);

private:
	static int baseHeight;
	static int surfaceHeights[World::TILES_W];
};