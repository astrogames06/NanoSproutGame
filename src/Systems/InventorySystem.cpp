#include "InventorySystem.hpp"

#include "../Player/Player.hpp"

#include "BuildingSystem.hpp"
#include "EatingSystem.hpp"

void RunInventorySystem()
{
    Player* player = game.GetEntityOfType<Player>();

    if (IsKeyPressed(KEY_ONE)) player->action_mode = ACTION_MODE::AXE;
    else if (IsKeyPressed(KEY_TWO)) player->action_mode = ACTION_MODE::EAT;
    else if (IsKeyPressed(KEY_THREE)) player->action_mode = ACTION_MODE::BUILD;

    switch (player->action_mode)
    {
    case ACTION_MODE::EAT:
        RunEatingSystem();
        break;
    case ACTION_MODE::BUILD:
        RunBuildingSystem();
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
    default:
        break;
    }
}

void DrawInventoryUI(Texture2D& inventory_tex)
{
    Player* player = game.GetEntityOfType<Player>();
    DrawTexture(inventory_tex, game.WIDTH/2-inventory_tex.width/2, game.HEIGHT-(inventory_tex.height+20), WHITE);
    DrawCircle(510+(player->action_mode*73), 785, 7, GREEN);
}