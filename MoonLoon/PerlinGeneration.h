#pragma once
#include "Common.h"

class PerlinGeneration
{
public:
	int gridSize = 1;
	static double PerlinNoise(double x, double y);
	static double DotGridGradient(int ix, int iy, double x, double y);
	static vector2 RandomGradient(int ix, int iy);
};

