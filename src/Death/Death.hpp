#ifndef DEATH_H
#define DEATH_H

#include "../Scene/Scene.hpp"
class Death : public Scene
{
    void Init() override;
    void Update() override;
    void DrawUI() override;
};

#endif