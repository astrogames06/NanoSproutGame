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
    BUILD,
    PLANT
};

struct DeathLoot
{
    Vector2 location;
    int wood = 0;
    int fruit = 0;
    int seeds = 0;
    bool available = false;

    DeathLoot() : location{0,0}, wood(0), fruit(0), seeds(0), available(false) {}
};

class Player : public Entity
{
public:    
    Vector2 velocity;
    Rectangle* current_axe_hitbox;
    Rectangle rect;
    Rectangle hit_box;
    Color tint;
    float air = 100.f;
    float health = 100.f;

    DeathLoot death_loot;
    bool has_died = false;   // Ever died
    bool just_died = false; 

    Sound player_hit;

    ACTION_MODE action_mode = AXE;
    DIRECTION direction = DOWN;

    int wood = 0;
    int fruit = 0;
    int seeds = 0;

    Player(float x, float y) {
        this->x = x;
        this->y = y;
    }

    void Init() override;
    void Update() override;
    void Draw() override;
    void Reset();
};

#endif