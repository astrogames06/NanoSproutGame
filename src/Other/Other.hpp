#ifndef OTHER_H
#define OTHER_H

#include "../Scene/Scene.hpp"
#include "../Game/Game.hpp"

#include "../Player/Player.hpp"

#include <iostream>

class Game;
extern Game game;

class Other : public Scene
{
    void Init() override
    {
        std::cout << "Other Scene initialized\n";
    }

    void Update() override
    {
        if (IsKeyPressed(KEY_SPACE))
        {
            std::unique_ptr<Player> new_entity = std::make_unique<Player>(0, 0);
            new_entity->x = GetRandomValue(0, game.WIDTH);
            new_entity->y = GetRandomValue(0, game.HEIGHT);
            game.AddEntity(std::move(new_entity));
        }
        std::cout << "Entities count: " << game.GetEntitiesOfType<Player>().size() << "\n";
    }

    void Draw() override
    {
        DrawText("Other Scene", 20, 20, 20, BLACK);
    }
};

#endif