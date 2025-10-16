#ifndef OTHER_H
#define OTHER_H

#include "../Scene/Scene.hpp"
class Menu : public Scene
{
    void Init() override;
    void Update() override;
    void DrawUI() override;
};

#endif