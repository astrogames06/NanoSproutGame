#include "Menu.hpp"

#include <iostream>
#include <raygui.h>

#include "../Game/Game.hpp"
#include "../Player/Player.hpp"
#include "../Main/Main.hpp"

class Game;
extern Game game;

namespace Scenes
{
    extern std::unique_ptr<Main> main_scene;
}

Texture2D play_button;

void Menu::Init()
{
    background_color = Color {210, 224, 119, 255};

    play_button = LoadTexture("assets/images/play_button.png");  
    play_button.width *= 4;
    play_button.height *= 4;

    Scenes::main_scene->music = LoadMusicStream("assets/sounds/song.wav");
    PlayMusicStream(Scenes::main_scene->music);
    Scenes::main_scene->music.looping = true;
}

void Menu::Update()
{
    UpdateMusicStream(Scenes::main_scene->music);
}

void Menu::DrawUI()
{
    if (CheckCollisionPointRec(GetMousePosition(), {(float)game.WIDTH/2-388/2, (float)400, 388, 132})
    && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        game.SetScene(Scenes::main_scene.get());
    }
    DrawTexture(play_button, game.WIDTH/2-play_button.width/2, 400, WHITE);
}