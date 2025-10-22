#include "InventorySystem.hpp"

#include <raylib.h>

#include "../Player/Player.hpp"

#include "BuildingSystem.hpp"
#include "EatingSystem.hpp"
#include "PlantingSystem.hpp"

#include <iostream>

Texture2D inventory_tex;

std::vector<Texture2D> inventory_texs;

void InitInventorySystem()
{
    inventory_tex = LoadTexture("assets/images/inventory.png");
    inventory_tex.width *= 1.5;
    inventory_tex.height *= 1.5;

    // Different inventory selects
    for (int i = 0; i < 4; i++)
    {
        std::string path = "assets/images/inventory_selects/" + std::to_string(i) + ".png";
        Texture2D tex = LoadTexture(path.c_str());
        tex.width *= 1.5;
        tex.height *= 1.5;

        inventory_texs.push_back(tex);
    }

    // Init planting system
    InitPlantingSystem();
}

void RunInventorySystem()
{
    Player* player = game.GetEntityOfType<Player>();

    if (IsKeyPressed(KEY_ONE)) player->action_mode = ACTION_MODE::AXE;
    else if (IsKeyPressed(KEY_TWO)) player->action_mode = ACTION_MODE::EAT;
    else if (IsKeyPressed(KEY_THREE)) player->action_mode = ACTION_MODE::BUILD;
    else if (IsKeyPressed(KEY_FOUR)) player->action_mode = ACTION_MODE::PLANT;

    switch (player->action_mode)
    {
    case ACTION_MODE::EAT:
        RunEatingSystem();
        break;
    case ACTION_MODE::BUILD:
        RunBuildingSystem();
        break;
    case ACTION_MODE::PLANT:
        RunPlantingSystem();
        break;
    default:
        break;
    }
}

void DrawInventorySystem()
{
    Player* player = game.GetEntityOfType<Player>();

    switch (player->action_mode)
    {
    case ACTION_MODE::BUILD:
        DrawBuildingSystem();
        break;
    case ACTION_MODE::PLANT:
        DrawPlantingSystem();
        break;
    default:
        break;
    }
}

void DrawInventoryUI()
{
    Player* player = game.GetEntityOfType<Player>();
    
    DrawTexture(inventory_texs[player->action_mode],
        game.WIDTH/2-inventory_texs[player->action_mode].width/2,
        game.HEIGHT-(inventory_texs[player->action_mode].height+20),
    WHITE);
    // DrawCircle(510+(player->action_mode*73), 785, 7, GREEN);
}