#include "EatingSystem.hpp"

#include "../Player/Player.hpp"

void RunEatingSystem()
{
    Player* player = game.GetEntityOfType<Player>();

    if (IsKeyPressed(KEY_E) || IsKeyPressed(KEY_SPACE))
    {
        if (player->fruit > 0)
        {
            player->fruit -= 1;
            player->health += 10;
        }
    }
}