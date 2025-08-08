#include "EnemySpawningSystem.hpp"

#include <raylib.h>
#include <vector>

#include "../Game/Game.hpp"
#include "../Main/Main.hpp"
#include "../Player/Player.hpp"
#include "../Enemy/Enemy.hpp"

extern Game game;

namespace Scenes {
    extern std::unique_ptr<Main> main_scene;
}

std::vector<Rectangle> rejected;
int spawn_enemy_delay = 0;

void RunEnemySpawningSystem()
{
    Player* player = game.GetEntityOfType<Player>();
    spawn_enemy_delay++;
    if (spawn_enemy_delay >= GetFPS()*3)
    {
        if (IsOnLand(player->rect, Scenes::main_scene->noise, game.CELL_SIZE))
        {
            std::unique_ptr<Enemy> new_enemy = std::make_unique<Enemy>(0, 0);
            int w = 30;
            int h = 30;

            int spawn_corner = GetRandomValue(0, 3);
            switch (spawn_corner)
            {
                case 0: // Top-left
                    new_enemy->x = player->x - game.WIDTH/2;
                    new_enemy->y = player->y - game.HEIGHT/2;
                    break;
                case 1: // Top-right
                    new_enemy->x = player->x + game.WIDTH/2;
                    new_enemy->y = player->y - game.HEIGHT/2;
                    break;
                case 2: // Bottom-left
                    new_enemy->x = player->x - game.WIDTH/2;
                    new_enemy->y = player->y + game.HEIGHT/2;
                    break;
                case 3: // Bottom-right
                    new_enemy->x = player->x + game.WIDTH/2;
                    new_enemy->y = player->y + game.HEIGHT/2;
                    break;
            }
            if (IsOnLand({
                (float)new_enemy->x, (float)new_enemy->y,
                (float)w, (float)h
            }, Scenes::main_scene->noise, game.CELL_SIZE))
            {
                //std::cout << "ADDED ENTITY: " << new_enemy->x << ", " << new_enemy->y << '\n';
                game.AddEntity(std::move(new_enemy));
            }
            else
            {
                //std::cout << "REJECTED ENTITY: " << new_enemy->x << ", " << new_enemy->y << '\n';
                rejected.push_back({
                    (float)new_enemy->x, (float)new_enemy->y,
                    (float)w, (float)h
                });
            }
        }

        spawn_enemy_delay = 0;
    }
}