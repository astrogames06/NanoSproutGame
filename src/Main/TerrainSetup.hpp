#ifndef TERRAINSETUP_H
#define TERRAINSETUP_H

#include <raylib.h>
#include "FastNoiseLite.h"
#include <memory>

#include "Main.hpp"

Texture2D tileAtlas;
Texture2D insidesAtlas;
Texture2D tree;
Texture2D bush;

int startX;
int startY;
int endX;
int endY;

namespace Scenes {
    extern std::unique_ptr<Main> main_scene;
}

void SetUpTerrain()
{
    Scenes::main_scene->noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    Scenes::main_scene->noise.SetFrequency(0.02f);
    Scenes::main_scene->noise.SetSeed(1337);

    tileAtlas = LoadTexture("assets/tiles.png");
    insidesAtlas = LoadTexture("assets/insides.png");
    tree = LoadTexture("assets/tree.png");
    bush = LoadTexture("assets/bush.png");
}

void UpdateTerrain()
{
    startX = (int)floor((game.camera.target.x - game.WIDTH / 2.0f) / game.CELL_SIZE) - 1;    
    startY = (int)floor((game.camera.target.y - game.HEIGHT / 2.0f) / game.CELL_SIZE) - 1;
    endX = startX + game.WIDTH / game.CELL_SIZE + 3;
    endY = startY + game.HEIGHT / game.CELL_SIZE + 3;
}

#endif