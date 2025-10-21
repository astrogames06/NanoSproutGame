#include "EatingSystem.hpp"

#include "../Player/Player.hpp"

void RunEatingSystem()
{
    Player* player = game.GetEntityOfType<Player>();

    if (IsKeyPressed(KEY_E) || IsKeyPressed(KEY_SPACE))
    {
        if (player->fruit > 0 && player->health < 100)
        {
            player->fruit--;
            player->seeds += 3;
            player->health += 10;
        }
    }
}