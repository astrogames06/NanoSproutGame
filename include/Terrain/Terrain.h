#pragma once

#include <raylib.h>
#include <iostream>
#include "FastNoiseLite.h"
#include "../../src/Entity/Entity.hpp"
#include <vector>

// Store tile grid positions of spawned plants to avoid duplicates
inline static std::vector<Vector2> used_positions;

inline bool positionUsed(const Vector2& pos) {
    for (const auto& p : used_positions) {
        if ((int)p.x == (int)pos.x && (int)p.y == (int)pos.y) return true;
    }
    return false;
}

enum PLANT_TYPE
{
    TREE,
    BUSH
};

struct Plant : public Entity {
    bool useTree = true;
    float scale = 2.5f;
    PLANT_TYPE type;

    void Init(FastNoiseLite& noise, Texture2D& tree, Texture2D& bush, int TILE_SIZE) {
        float val = (noise.GetNoise((float)x / TILE_SIZE, (float)y / TILE_SIZE) + 1.0f) / 2.0f;
        if (val <= 0.5f) {
            alive = false;
            return;
        }
        unsigned int hash = ((unsigned int)((x / TILE_SIZE) * 19349663) ^ ((y / TILE_SIZE) * 83492791));
        useTree = (hash & 1) == 0;
        texture = useTree ? tree : bush;
    }

    void Draw() override {
        DrawTextureEx(texture, {(float)x, (float)y}, 0.0f, scale, WHITE);
    }
};

const int ATLAS_TILE_SIZE = 16;

inline bool HasNeighbor(int x, int y, FastNoiseLite& noise) {
    float val = (noise.GetNoise((float)x, (float)y) + 1.0f) / 2.0f;
    return val > 0.5f;
}

inline int GetTileIndex(int x, int y, FastNoiseLite& noise) {
    bool top    = HasNeighbor(x, y - 1, noise);
    bool right  = HasNeighbor(x + 1, y, noise);
    bool bottom = HasNeighbor(x, y + 1, noise);
    bool left   = HasNeighbor(x - 1, y, noise);

    if (!top && !left)         return 0; // top-left corner
    if (!top && left && right) return 1; // top edge
    if (!top && !right)        return 2; // top-right corner
    if (!left && top && bottom) return 3; // left edge
    if (top && bottom && left && right) return 4; // center
    if (!right && top && bottom) return 5; // right edge
    if (!bottom && !left)      return 6; // bottom-left corner
    if (!bottom && left && right) return 7; // bottom edge
    if (!bottom && !right)     return 8; // bottom-right corner

    return 4;
}

inline void DrawAutoTile(int x, int y, Texture2D atlas, FastNoiseLite& noise, int TILE_SIZE) {
    float val = (noise.GetNoise((float)x, (float)y) + 1.0f) / 2.0f;
    if (val <= 0.5f) return;

    int index = GetTileIndex(x, y, noise);
    int tx = (index % 3) * ATLAS_TILE_SIZE;
    int ty = (index / 3) * ATLAS_TILE_SIZE;
    Rectangle src = { (float)tx, (float)ty, (float)ATLAS_TILE_SIZE, (float)ATLAS_TILE_SIZE };
    Vector2 pos = { (float)(x * TILE_SIZE), (float)(y * TILE_SIZE) };
    Rectangle dest = { pos.x, pos.y, (float)TILE_SIZE, (float)TILE_SIZE };
    DrawTexturePro(atlas, src, dest, {0, 0}, 0.0f, WHITE);
}

inline void DrawInsideCorners(int x, int y, Texture2D insides, FastNoiseLite& noise, int TILE_SIZE) {
    bool top    = HasNeighbor(x, y - 1, noise);
    bool right  = HasNeighbor(x + 1, y, noise);
    bool bottom = HasNeighbor(x, y + 1, noise);
    bool left   = HasNeighbor(x - 1, y, noise);

    bool topLeft     = HasNeighbor(x - 1, y - 1, noise);
    bool topRight    = HasNeighbor(x + 1, y - 1, noise);
    bool bottomLeft  = HasNeighbor(x - 1, y + 1, noise);
    bool bottomRight = HasNeighbor(x + 1, y + 1, noise);

    Vector2 pos = { (float)(x * TILE_SIZE), (float)(y * TILE_SIZE) };
    Rectangle dest = { pos.x, pos.y, (float)TILE_SIZE, (float)TILE_SIZE };

    if (top && left && !topLeft) {
        Rectangle src = { (float)ATLAS_TILE_SIZE, (float)ATLAS_TILE_SIZE, (float)ATLAS_TILE_SIZE, (float)ATLAS_TILE_SIZE };
        DrawTexturePro(insides, src, dest, {0, 0}, 0.0f, WHITE);
    }
    if (top && right && !topRight) {
        Rectangle src = { 0, (float)ATLAS_TILE_SIZE, (float)ATLAS_TILE_SIZE, (float)ATLAS_TILE_SIZE };
        DrawTexturePro(insides, src, dest, {0, 0}, 0.0f, WHITE);
    }
    if (bottom && left && !bottomLeft) {
        Rectangle src = { (float)ATLAS_TILE_SIZE, 0, (float)ATLAS_TILE_SIZE, (float)ATLAS_TILE_SIZE };
        DrawTexturePro(insides, src, dest, {0, 0}, 0.0f, WHITE);
    }
    if (bottom && right && !bottomRight) {
        Rectangle src = { 0, 0, (float)ATLAS_TILE_SIZE, (float)ATLAS_TILE_SIZE };
        DrawTexturePro(insides, src, dest, {0, 0}, 0.0f, WHITE);
    }
}

inline void DrawTerrainAndPlants(FastNoiseLite noise, std::vector<std::unique_ptr<Entity>>* entities,
    Vector2 start, Vector2 end,
    Texture2D tileAtlas, Texture2D insidesAtlas,
    Texture2D tree, Texture2D bush,
    float plant_scale, int TILE_SIZE)
{

    for (int y = (int)start.y; y < (int)end.y; y++) {
        for (int x = (int)start.x; x < (int)end.x; x++) {
            DrawAutoTile(x, y, tileAtlas, noise, TILE_SIZE);
            DrawInsideCorners(x, y, insidesAtlas, noise, TILE_SIZE);

            float val = (noise.GetNoise((float)x, (float)y) + 1.0f) / 2.0f;
            float vTop = (noise.GetNoise((float)x, (float)(y - 1)) + 1.0f) / 2.0f;
            float vBot = (noise.GetNoise((float)x, (float)(y + 1)) + 1.0f) / 2.0f;
            float vLeft = (noise.GetNoise((float)(x - 1), (float)y) + 1.0f) / 2.0f;
            float vRight = (noise.GetNoise((float)(x + 1), (float)y) + 1.0f) / 2.0f;

            if (val > 0.55f && vTop > 0.55f && vBot > 0.55f && vLeft > 0.55f && vRight > 0.55f) {
                unsigned int hash = ((unsigned int)(x * 73856093) ^ (y * 19349663)) % 100;
                if (hash < 10 && x % 3 == 0 && y % 5 == 0) {
                    Vector2 tilePos = { (float)x, (float)y };
                    if (!positionUsed(tilePos)) {
                        unsigned int hash2 = ((unsigned int)(x * 19349663) ^ (y * 83492791));
                        bool useTree = (hash2 & 1) == 0;

                        Vector2 posPixels = {
                            x * TILE_SIZE + TILE_SIZE - (tree.width * plant_scale),
                            y * TILE_SIZE + TILE_SIZE - (tree.height * plant_scale)
                        };

                        std::unique_ptr<Plant> new_plant = std::make_unique<Plant>();
                        new_plant->x = (int)posPixels.x;
                        new_plant->y = (int)posPixels.y;
                        new_plant->useTree = useTree;
                        new_plant->texture = useTree ? tree : bush;
                        new_plant->type = useTree ? PLANT_TYPE::TREE : PLANT_TYPE::BUSH;
                        new_plant->scale = plant_scale;

                        entities->push_back(std::move(new_plant));
                        used_positions.push_back(tilePos);
                    }
                }
            }
        }
    }

    // Draw all plants
    for (const auto& entity : *entities) {
        Plant* plant = dynamic_cast<Plant*>(entity.get());
        if (plant) {
            plant->Draw();
        }
    }
}

inline bool IsOnLand(Rectangle rect, FastNoiseLite& noise, int TILE_SIZE) {
    if (rect.width <= 0 || rect.height <= 0) {
        return false;
    }

    int startX = (int)(rect.x / TILE_SIZE);
    int endX = (int)((rect.x + rect.width) / TILE_SIZE);
    int startY = (int)(rect.y / TILE_SIZE);
    int endY = (int)((rect.y + rect.height) / TILE_SIZE);

    int landCount = 0;
    int sampleCount = 0;

    for (int y = startY; y < endY; y++) {
        for (int x = startX; x < endX; x++) {
            float val = (noise.GetNoise((float)x, (float)y) + 1.0f) / 2.0f;
            if (val > 0.5f) landCount++;
            sampleCount++;
        }
    }

    return ((float)landCount / sampleCount) >= 0.5f;
}
