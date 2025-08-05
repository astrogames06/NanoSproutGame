#ifndef DOOR_H
#define DOOR_H

#include "../Entity/Entity.hpp"
#include "../Block/Block.hpp"

struct Door : public Block
{
public:
    Door(float x, float y)
    {
        this->x = x;
        this->y = y;
    }

    void Draw() override;
};

#endif