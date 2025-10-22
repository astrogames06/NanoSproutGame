#ifndef CROP_H
#define CROP_H

#include "../Entity/Entity.hpp"

class Crop : public Entity
{
public:
    int points = 0;

    void Init() override;
    void Update() override;
    void Draw() override;
};

#endif