#include "Crop.hpp"

void Crop::Init()
{
    texture = LoadTexture("assets/images/crop/0.png");
}

void Crop::Update()
{
    ;
}

void Crop::Draw()
{
    DrawTexture(texture, x, y, WHITE);
}