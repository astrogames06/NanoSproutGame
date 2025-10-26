#include "Menu.hpp"

#include <iostream>
#include <raygui.h>

#include "../Game/Game.hpp"
#include "../Player/Player.hpp"
#include "../Main/Main.hpp"
#include "../Settings/Settings.hpp"

class Game;
extern Game game;

namespace Scenes
{
    extern std::unique_ptr<Main> main_scene;
    extern std::unique_ptr<Settings> settings_scene;
}

Texture2D title;

Texture2D play_button;
Texture2D settings_button;
Texture2D exit_button;

void Menu::Init()
{
    background_color = Color {210, 224, 119, 255};

    title = LoadTexture("assets/images/logo.png");

    play_button = LoadTexture("assets/images/play_button.png");  
    play_button.width *= 4;
    play_button.height *= 4;

    settings_button = LoadTexture("assets/images/settings_button.png");  
    settings_button.width *= 4;
    settings_button.height *= 4;

    exit_button = LoadTexture("assets/images/exit_btn.png");  
    exit_button.width *= 4;
    exit_button.height *= 4;

    if (Scenes::main_scene->music.stream.buffer == NULL)
    {
        Scenes::main_scene->music = LoadMusicStream("assets/sounds/song.wav");
        PlayMusicStream(Scenes::main_scene->music);
        Scenes::main_scene->music.looping = true;
    }

    SetMusicVolume(Scenes::main_scene->music, settings_music_volume);
}

void Menu::Update()
{
    UpdateMusicStream(Scenes::main_scene->music);
}

void Menu::DrawUI()
{
    // Draw title
    DrawTexture(title, game.WIDTH/2-title.width/2, 100, WHITE);

    // Play button
    if (CheckCollisionPointRec(GetMousePosition(), {(float)game.WIDTH/2-388/2, (float)300, 388, 132})
    && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        game.SetScene(Scenes::main_scene.get());
    }
    DrawTexture(play_button, game.WIDTH/2-play_button.width/2, 300, WHITE);

    // Settings button
    if (CheckCollisionPointRec(GetMousePosition(), {(float)game.WIDTH/2-388/2, (float)500, 388, 132})
    && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        game.SetScene(Scenes::settings_scene.get());
    }
    DrawTexture(settings_button, game.WIDTH/2-settings_button.width/2, 500, WHITE);

    // Exit button
    if (CheckCollisionPointRec(GetMousePosition(), {(float)game.WIDTH/2-388/2, (float)700, 388, 132})
    && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        game.Exit(); // Closes game
    }
    DrawTexture(exit_button, game.WIDTH/2-exit_button.width/2, 700, WHITE);
}