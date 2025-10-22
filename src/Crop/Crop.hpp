#ifndef CROP_H
#define CROP_H

#include <chrono>
#include "../Entity/Entity.hpp"

class Crop : public Entity
{
public:
    bool ready_to_harvest = false;
    int points = 0;

    void Init() override;
    void Update() override;
    void Draw() override;
private:
    std::chrono::high_resolution_clock::time_point last_time;
};

#endif