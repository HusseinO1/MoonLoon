#include "Player.h"
#include "WinBuffer.h"
#include "CustomMath.h"


Player::Player(double x, double y, double r, World& wrld) : world{ wrld }
{
	centralPos = { x, y };
	velocity = { 0,0 };
	playerRadius = r;
    worldSpawnPos = centralPos;
}
void Player::Draw(const Camera& camera) const
{
    const int radius = static_cast<int>(playerRadius);
    const int screenCenterX = static_cast<int>(centralPos.x - camera.x);
    const int screenCenterY = static_cast<int>(centralPos.y - camera.y);
    const int radiusSquared = radius * radius;

    for(int y = -radius; y <= radius; ++y)
    {
        for(int x = -radius; x <= radius; ++x)
        {
            if(x * x + y * y <= radiusSquared)
            {
                WinBuffer::DrawAtSafe(screenCenterX + x, screenCenterY + y, 230, 40, 40);
            }
        }
    }
}

void Player::Update(World& wrld)
{
    world = wrld;
    GravitationalLogic();
    JumpLogic();
    MovementLogic();
    MoveX(world);
    MoveY(world);
    CheckSpikes(world);
}

void Player::GravitationalLogic()
{
    velocity.y += gravity;
    CustomMath::Clamp(velocity.y, 0, maxFallSpeed);

    centralPos.y += velocity.y * deltaTime;
}

void Player::MovementLogic()
{
    float moveDirection = 0.0f;

    if(centralPos.x > (1.50 * playerRadius) && GetAsyncKeyState('A') & 0x8000)
    {
        moveDirection -= 1.0f;
    }
    if(centralPos.x < World::TILES_W * TILE_SIZE - WinBuffer::w && GetAsyncKeyState('D') & 0x8000)
    {
        moveDirection += 1.0f;
    }

    velocity.x = moveDirection * playerSpeed;

    centralPos.x += (!onGround) ? 0.85f * velocity.x * deltaTime: velocity.x * deltaTime;
}

void Player::JumpLogic()
{
    const bool spaceDown = (GetAsyncKeyState(VK_SPACE) & 0x8000) != 0;

    if(onGround && spaceDown && !jumpedLastFrame)
    {
        onGround = false;
        velocity.y = -jumpSpeed;
    }
    else if(!spaceDown && velocity.y < 0.0)
    {
        velocity.y *= 0.5;
    }

    jumpedLastFrame = spaceDown;
}

bool Player::IsSolid(World& world, int col, int row) const
{
    return world.GetTile(col, row).type != TileType::Air;
}

bool Player::ResolveTile(World& world, int col, int row, vector2& normal)
{
    const double left = col * TILE_SIZE;
    const double right = left + TILE_SIZE;
    const double top = row * TILE_SIZE;
    const double bottom = top + TILE_SIZE;

    if(centralPos.x > right && IsSolid(world, col + 1, row)) return false;
    if(centralPos.x < left && IsSolid(world, col - 1, row)) return false;
    if(centralPos.y > bottom && IsSolid(world, col, row + 1)) return false;
    if (centralPos.y < top && IsSolid(world, col, row - 1)) return false;

    const double closestX = CustomMath::Clamp(centralPos.x, left, right);
    const double closestY = CustomMath::Clamp(centralPos.y, top, bottom);

    const double dx = centralPos.x - closestX;
    const double dy = centralPos.y - closestY;
    const double distSq = dx * dx + dy * dy;

    if(distSq >= playerRadius * playerRadius) return false;

    if(distSq < 0.0001)
    {
        normal = { 0.0, -1.0 };
        centralPos.y = top - playerRadius;
        return true;
    }

    const double dist = CustomMath::Sqrt(distSq);
    const double push = playerRadius - dist;

    normal = { dx / dist, dy / dist };
    centralPos.x += normal.x * push;
    centralPos.y += normal.y * push;
    return true;
}

bool Player::ResolveNearbyTiles(World& world, vector2& normalSum)
{
    normalSum = { 0.0, 0.0 };
    bool hit = false;

    const int minCol = static_cast<int>(CustomMath::Floor((centralPos.x - playerRadius) / TILE_SIZE));
    const int maxCol = static_cast<int>(CustomMath::Floor((centralPos.x + playerRadius) / TILE_SIZE));
    const int minRow = static_cast<int>(CustomMath::Floor((centralPos.y - playerRadius) / TILE_SIZE));
    const int maxRow = static_cast<int>(CustomMath::Floor((centralPos.y + playerRadius) / TILE_SIZE));

    for (int row = minRow; row <= maxRow; ++row)
    {
        for (int col = minCol; col <= maxCol; ++col)
        {
            if (!IsSolid(world, col, row)) continue;

            vector2 n;
            if (ResolveTile(world, col, row, n))
            {
                normalSum.x += n.x;
                normalSum.y += n.y;
                hit = true;
            }
        }
    }
    return hit;
}

void Player::MoveX(World& world)
{
    centralPos.x += velocity.x * deltaTime;

    vector2 n;
    if (ResolveNearbyTiles(world, n) && CustomMath::Abs(n.x) > 0.5)
    {
        velocity.x = 0.0;
    }
}

void Player::MoveY(World& world)
{
    onGround = false;
    centralPos.y += velocity.y * deltaTime;

    vector2 n;
    if (ResolveNearbyTiles(world, n))
    {
        if (n.y < -0.5) 
        {
            onGround = true;
            if (velocity.y > 0.0) velocity.y = 0.0;
        }
        else if (n.y > 0.5) 
        {
            if (velocity.y < 0.0) velocity.y = 0.0;
        }
    }
}
void Player::Respawn()
{
    centralPos = worldSpawnPos;
    velocity = { 0.0, 0.0 };
}

void Player::CheckSpikes(World& world)
{
    const int minCol = static_cast<int>(CustomMath::Floor((centralPos.x - playerRadius) / TILE_SIZE));
    const int maxCol = static_cast<int>(CustomMath::Floor((centralPos.x + playerRadius) / TILE_SIZE));
    const int minRow = static_cast<int>(CustomMath::Floor((centralPos.y - playerRadius) / TILE_SIZE));
    const int maxRow = static_cast<int>(CustomMath::Floor((centralPos.y + playerRadius) / TILE_SIZE));

    for (int row = minRow; row <= maxRow; ++row)
    {
        for (int col = minCol; col <= maxCol; ++col)
        {
            Tile t = world.GetTile(col, row);
            if (!t.spiked) continue;

            // Compute bounding box / closest point of the spiked tile
            const double left = col * TILE_SIZE;
            const double right = left + TILE_SIZE;
            const double top = row * TILE_SIZE;
            const double bottom = top + TILE_SIZE;

            const double closestX = CustomMath::Clamp(centralPos.x, left, right);
            const double closestY = CustomMath::Clamp(centralPos.y, top, bottom);

            const double dx = centralPos.x - closestX;
            const double dy = centralPos.y - closestY;

            // Circle-AABB intersection check
            if ((dx * dx + dy * dy) < (playerRadius * playerRadius))
            {
                Respawn();
                return;
            }
        }
    }
}
