#ifndef ENEMY_H
#define ENEMY_H

#include "../Game/Game.hpp"
#include "../Entity/Entity.hpp"

class Enemy : public Entity
{
public:    
    bool follow_player = false;

    Vector2 velocity;
    float acceleration = 1.f;
    float speed = 3.f;

    float health = 100.f;

    Enemy(float x, float y) {
        this->x = x;
        this->y = y;
    }

    void Init() override;
    void Update() override;
    void Draw() override;
};

#endif