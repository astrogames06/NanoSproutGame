#ifndef MAIN_H
#define MAIN_H

#include "../Scene/Scene.hpp"
#include "../Game/Game.hpp"
#include "Terrain/Terrain.h"

#include "../Player/Player.hpp"
#include "../Enemy/Enemy.hpp"

#include <iostream>

extern Game game;

class Main : public Scene
{
    FastNoiseLite noise;

    Texture2D tileAtlas;
    Texture2D insidesAtlas;
    Texture2D tree;
    Texture2D bush;

    int startX;
    int startY;
    int endX;
    int endY;

    void Init() override
    {
        game.CELL_SIZE = 64;
        background_color = Color{155, 212, 195, 255};
        std::cout << "Main Scene initialized\n";
        std::unique_ptr<Player> player = std::make_unique<Player>(100, 100);
        game.AddEntity(std::move(player));

        FastNoiseLite noise;
        noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
        noise.SetFrequency(0.02f);
        noise.SetSeed(1337);

        tileAtlas = LoadTexture("assets/tiles.png");
        insidesAtlas = LoadTexture("assets/insides.png");
        tree = LoadTexture("assets/tree.png");
        bush = LoadTexture("assets/bush.png");
    }

    void Update() override
    {
        startX = (int)floor((game.camera.target.x - game.WIDTH / 2.0f) / game.CELL_SIZE) - 1;    
        startY = (int)floor((game.camera.target.y - game.HEIGHT / 2.0f) / game.CELL_SIZE) - 1;
        endX = startX + game.WIDTH / game.CELL_SIZE + 3;
        endY = startY + game.HEIGHT / game.CELL_SIZE + 3;

        Player* player = game.GetEntityOfType<Player>();
        game.camera.target.x = player->x;
        game.camera.target.y = player->y;
        game.camera.offset = {(float)game.WIDTH/2, (float)game.HEIGHT/2};

        if (IsKeyPressed(KEY_O))
        {
            std::unique_ptr<Enemy> new_enemy = std::make_unique<Enemy>(0, 0);
            new_enemy->x = GetRandomValue(0, game.WIDTH);
            new_enemy->y = GetRandomValue(0, game.HEIGHT);
            game.AddEntity(std::move(new_enemy));
        }

        for (Plant* plant : game.GetEntitiesOfType<Plant>())
        {
            // if (!plant->remove)
            //     std::cout << "Plant at " << plant->x << ", " << plant->y << " is alive\n";
            // else
            //     std::cout << "Plant at " << plant->x << ", " << plant->y << " marked for removal\n";

            if (CheckCollisionRecs(*player->current_axe_hitbox,
                {(float)plant->x, (float)plant->y, (float)plant->texture.width*plant->scale, (float)plant->texture.height*plant->scale}
            ))
            {
                // std::cout << "Deleting plant at " << plant->x << ", " << plant->y << "\n";
                if (plant->type == PLANT_TYPE::TREE)
                {
                    player->wood += 3;
                }
                else if (plant->type == PLANT_TYPE::BUSH)
                {
                    player->wood++;
                    player->fruit++;
                }
                plant->Delete();
            }
        }
    }

    void Draw() override
    {
        DrawTerrainAndPlants(noise, &entities,
            {(float)startX, (float)startY}, {(float)endX, (float)endY},
            tileAtlas, insidesAtlas,
            tree, bush, 2.5f,
            game.CELL_SIZE
        );
    }
};

#endif