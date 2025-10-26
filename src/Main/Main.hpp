#ifndef MAIN_H
#define MAIN_H

#include "../Scene/Scene.hpp"
#include "../Game/Game.hpp"
#include "Terrain/Terrain.h"

#include "../Player/Player.hpp"
#include "../Enemy/Enemy.hpp"

#include <iostream>

extern Game game;

class Main : public Scene
{
public:
    FastNoiseLite noise;
    Font customFont;
    Music music;

    Sound block_sound;
   
    void Init() override;
    void Update() override;
    void Draw() override;
    void DrawUI() override;

    void DrawStats();
};

#endif