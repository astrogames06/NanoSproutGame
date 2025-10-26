#include "Death.hpp"

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

void Death::Init()
{
    background_color = BLACK;
}

void Death::Update()
{
    UpdateMusicStream(Scenes::main_scene->music);
}

void Death::DrawUI()
{
    int title_width = MeasureText("You Died!", 80);
    DrawTextEx(Scenes::main_scene->customFont, "You Died!", {(float)game.WIDTH/2-title_width/2, 50}, 80, 2, WHITE);
    
    if (GuiButton({(float)game.WIDTH/2-388/2, (float)400, 388, 132}, "HOME!"))
    {
        Player* player = game.GetEntityInOtherScene<Player>(Scenes::main_scene.get());
        if (player)
        {
            player->Reset();
        }

        game.SetScene(Scenes::menu_scene.get());
    }
}