#ifndef PLAYER_H
#define PLAYER_H

#include "../Game/Game.hpp"
#include "../Entity/Entity.hpp"

extern Game game;

enum DIRECTION
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

enum ACTION_MODE
{
    AXE,
    EAT,
    BUILD
};

class Player : public Entity
{
public:    
    Vector2 velocity;
    Rectangle* current_axe_hitbox;
    Rectangle rect;
    Rectangle hit_box;
    Color tint;
    float air;
    float health;

    ACTION_MODE action_mode;
    DIRECTION direction;

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