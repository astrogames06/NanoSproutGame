#ifndef BLOCK_H
#define BLOCK_H

#include "../Entity/Entity.hpp"

struct Block : public Entity
{
public:
    int x;
    int y;
    int width = 50;
    int height = 50;

    Block(float x, float y) {
        this->x = x;
        this->y = y;
    }
    
    void Draw() override;
};

#endif