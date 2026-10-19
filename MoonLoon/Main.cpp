#include "Window.h"
#include "WinBuffer.h"
#include "Common.h"
#include "World.h"
#include "WorldGen.h"
#include "Player.h"
#include "Spike.h"

void CamFollow(Camera& camera, Player& player);

void DisplayBuffer(World* world, Window* pWindow, const Camera& camera, const Player& player);

int main()
{
    World* world = new World();
    WorldGen::GenerateWorld(*world);

    Window* pWindow = new Window();

    Camera camera{
        0.0f,
        WorldGen::GetSurfaceHeight(16) * static_cast<float>(TILE_SIZE) - WinBuffer::h / 2.0f - 200.0f
    };

    uint64 previousFrame = GetTickCount64();

    vector2 playerSpawn = { 16 * TILE_SIZE, WorldGen::GetSurfaceHeight(16) * TILE_SIZE - 20.0 };

    Player player = Player(
        playerSpawn.x,
        playerSpawn.y,
        20.0,
        *world);



    bool running = true;
    while (running)
    {
        if (!pWindow->ProcessMessages())
        {
            running = false;
            break;
        }

        uint64 currentFrame = GetTickCount64();
        float deltaSeconds = static_cast<float>(currentFrame - previousFrame) / 1000.0f;
        previousFrame = currentFrame;

        
        if (deltaSeconds > 0.1f)
        {
            deltaSeconds = 0.1f;
        }

        //THIS IS WHERE MY ACTUAL GAME LOOP LOGIC STARTS
        player.Update(*world);

        CamFollow(camera, player);
        

        DisplayBuffer(world, pWindow, camera, player);

        totalTime += deltaSeconds;
        Sleep(1);
    }

    delete pWindow;
    delete world;
    return 0;
}

void CamFollow(Camera& camera, Player& player)
{
    camera.x = static_cast<float>(player.centralPos.x - WinBuffer::w / 2.0);
    camera.y = static_cast<float>(player.centralPos.y - WinBuffer::h / 2.0);

    const float maxX = static_cast<float>(World::TILES_W * TILE_SIZE - WinBuffer::w);
    const float maxY = static_cast<float>(World::TILES_H * TILE_SIZE - WinBuffer::h);
    if(camera.x < 0.0f) camera.x = 0.0f;
    if(camera.x > maxX) camera.x = maxX;
    if(camera.y < 0.0f) camera.y = 0.0f;
    if(camera.y > maxY) camera.y = maxY;
}


void DisplayBuffer(World* world, Window* pWindow, const Camera& camera, const Player& player)
{
    const int camX = static_cast<int>(camera.x);
    const int camY = static_cast<int>(camera.y);
    const int firstX = camX / TILE_SIZE;
    const int firstY = camY / TILE_SIZE;
    const int across = WinBuffer::w / TILE_SIZE + 2;
    const int down = WinBuffer::h / TILE_SIZE + 2;

    WinBuffer::Clear(0, 0, 0);

   
    const int STAR_COUNT = 150;
    for (int i = 0; i < STAR_COUNT; ++i)
    {
        const unsigned hash = static_cast<unsigned>(i + 1) * 2654435761u;
        int sx = static_cast<int>(hash % static_cast<unsigned>(WinBuffer::w));
        int sy = static_cast<int>((hash >> 11) % static_cast<unsigned>(WinBuffer::h));

        sx = ((sx - camX / 10) % WinBuffer::w + WinBuffer::w) % WinBuffer::w;
        sy = ((sy - camY / 10) % WinBuffer::h + WinBuffer::h) % WinBuffer::h;

        const int b = 140 + static_cast<int>((hash >> 20) % 116u); 
        const int size = (i % 7 == 0) ? 2 : 1;            
        WinBuffer::FillRect(sx, sy, size, size, b, b, b);
    }
    int hoverX = -1;
    int hoverY = -1;

    POINT cursor;
    RECT client;
    if(GetCursorPos(&cursor) && ScreenToClient(pWindow->GetHWND(), &cursor) && GetClientRect(pWindow->GetHWND(), &client))
    {
        const int clientW = client.right - client.left;
        const int clientH = client.bottom - client.top;

        if(clientW > 0 && clientH > 0 &&
            cursor.x >= 0 && cursor.y >= 0 &&
            cursor.x < clientW && cursor.y < clientH)
        {
            const int bufX = cursor.x * WinBuffer::w / clientW;
            const int bufY = cursor.y * WinBuffer::h / clientH;

            hoverX = (bufX + camX) / TILE_SIZE;
            hoverY = (bufY + camY) / TILE_SIZE;
        }
    }

    for(int wy = firstY; wy < firstY + down; ++wy)
    {
        for(int wx = firstX; wx < firstX + across; ++wx)
        {
            const Tile tile = world->GetTile(wx, wy);
            if (tile.spiked)
                Spike::DrawSpike(wx, wy, camera);
            if(tile.type == TileType::Air)
            {
                continue;
            }

            const Color color = TILE_COLORS[static_cast<int>(tile.type)];
            const int screenX = wx * TILE_SIZE - camX;
            const int screenY = wy * TILE_SIZE - camY;

            WinBuffer::FillRect(
                screenX, screenY, TILE_SIZE, TILE_SIZE,
                color.r, color.g, color.b);

            const bool hovered = (wx == hoverX && wy == hoverY);
            const int bc = hovered ? 255 : 0;

            WinBuffer::FillRect(screenX, screenY, TILE_SIZE, 1, bc, bc, bc);
            WinBuffer::FillRect(
                screenX, screenY + TILE_SIZE - 1, TILE_SIZE, 1, bc, bc, bc);
            WinBuffer::FillRect(screenX, screenY, 1, TILE_SIZE, bc, bc, bc);
            WinBuffer::FillRect(
                screenX + TILE_SIZE - 1, screenY, 1, TILE_SIZE, bc, bc, bc);
        }
    }

    player.Draw(camera);

    InvalidateRect(pWindow->GetHWND(), nullptr, FALSE);
}