#ifndef UI_H
#define UI_H

#include <raylib.h>

#include "../Player/Player.hpp"

Font customFont;
Texture2D tree_icon;
Texture2D fruit_icon;

void InitUI()
{
    tree_icon = LoadTexture("assets/wood_icon.png");
    tree_icon.width *= 3;
    tree_icon.height *= 3;
    fruit_icon = LoadTexture("assets/fruit_icon.png");
    fruit_icon.width *= 3;
    fruit_icon.height *= 3;

    customFont = LoadFontEx("assets/pixel_font.ttf", 48, nullptr, 0);
}

void DrawStats()
{
    Player* player = game.GetEntityOfType<Player>();
    
    DrawTexture(tree_icon, 20, 20, WHITE);
    //DrawText(std::to_string(player->wood).c_str(), 80, 20, 48, BLACK);
    DrawTextEx(customFont, std::to_string(player->wood).c_str(), (Vector2){ 80, 20 }, 48, 2, WHITE);

    DrawTexture(fruit_icon, 20, 70, WHITE);
    //DrawText(std::to_string(player->wood).c_str(), 80, 20, 48, BLACK);
    DrawTextEx(customFont, std::to_string(player->fruit).c_str(), (Vector2){ 80, 70 }, 48, 2, WHITE);
}

void DrawUI()
{
    DrawStats();
}

#endif