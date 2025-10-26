#include "Settings.hpp"

#include <iostream>
#include <raygui.h>

#include "../Game/Game.hpp"
#include "../Player/Player.hpp"
#include "../Main/Main.hpp"
#include "../Menu/Menu.hpp"

class Game;
extern Game game;

namespace Scenes
{
    extern std::unique_ptr<Main> main_scene;
    extern std::unique_ptr<Menu> menu_scene;
}

Texture2D home_button;

void Settings::Init()
{
    background_color = Color {210, 224, 119, 255};

    home_button = LoadTexture("assets/images/home_button.png");  
    home_button.width *= 4;
    home_button.height *= 4;
}

void Settings::Update()
{
    UpdateMusicStream(Scenes::main_scene->music);
}

void Settings::DrawUI()
{   
    // Player color picker
    GuiColorPicker({(float)game.WIDTH/2-100/2, 100, 100, 100}, "Color!", &player_selected_color);
    Player* player = game.GetEntityInOtherScene<Player>(Scenes::main_scene.get());
    if (player != nullptr) player->character_color = player_selected_color;

    // Music volume
    GuiSlider({(float)game.WIDTH/2 - 150, 400, 300, 50}, "Volume", TextFormat("%.2f", settings_music_volume), &settings_music_volume, 0.0f, 10.0f);
    SetMusicVolume(Scenes::main_scene->music, settings_music_volume);

    // Home button
    if (CheckCollisionPointRec(GetMousePosition(),
        {(float)game.WIDTH/2-home_button.width/2, (float)600, (float)home_button.width, (float)home_button.height}
    )
    && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        game.SetScene(Scenes::menu_scene.get());
    }
    DrawTexture(home_button, game.WIDTH/2-home_button.width/2, 600, WHITE);
}