#pragma once
#include "Common.h"
#include "World.h"
#include "WinBuffer.h"
class Player
{
public:
    vector2 centralPos;
    vector2 velocity;
    vector2 worldSpawnPos;
    double playerSpeed = 4.5 * TILE_SIZE;
    double gravity = TILE_SIZE / 10;
    double jumpSpeed = 4 * TILE_SIZE;
    double maxFallSpeed = 5 * gravity;
    double playerRadius;
    bool onGround = false;
	bool jumpedLastFrame = false;
    World& world;
    Player(double x, double y, double r, World& wrld);
    void Draw(const Camera& camera) const;

    void Update(World& wrld);
    void GravitationalLogic();
    void MovementLogic();
    void JumpLogic();
    void Respawn();
    void CheckSpikes(World& world);

private:
    bool IsSolid(World& world, int col, int row) const;
    bool ResolveTile(World& world, int col, int row, vector2& normal);
    bool ResolveNearbyTiles(World& world, vector2& normalSum);
    void MoveX(World& world);
    void MoveY(World& world);
};

