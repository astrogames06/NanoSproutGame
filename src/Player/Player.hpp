#ifndef PLAYER_H
#define PLAYER_H

#include "../Game/Game.hpp"
#include "../Entity/Entity.hpp"

extern Game game;

class Player : public Entity
{
public:    
    Rectangle* current_axe_hitbox;
    Rectangle rect;
    Color tint;

    int wood = 0;
    int fruit = 0;

    Player(float x, float y) {
        this->x = x;
        this->y = y;
    }

    void Init() override;
    void Update() override;
    void Draw() override;
};

#endif