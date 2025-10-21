#include "Main.hpp"

#include "TerrainSetup.hpp"

#include "../Systems/InventorySystem.hpp"
#include "../Systems/EnemySpawningSystem.hpp"

#include "../Menu/Menu.hpp"

#include "../Data/Data.hpp"

Font customFont;
Texture2D tree_icon;
Texture2D fruit_icon;
Texture2D seed_icon;

Sound tree_hit;
Sound bush_hit;

Texture2D pointer;
Texture2D home_button_tex;

namespace Scenes
{
    extern std::unique_ptr<Menu> menu_scene;
}

void Main::Init()
{
    std::cout << "INITTING!!\n";
    game.CELL_SIZE = 64;
    background_color = Color{155, 212, 195, 255};
    std::cout << "Main Scene initialized\n";

    if (game.GetEntityOfType<Player>() == nullptr)
    {
        std::unique_ptr<Player> player = std::make_unique<Player>(100, 100);
        game.AddEntity(std::move(player));
    }

    SetUpTerrain();
    InitInventorySystem();

    tree_icon = LoadTexture("assets/images/wood_icon.png");
    tree_icon.width *= 3;
    tree_icon.height *= 3;
    fruit_icon = LoadTexture("assets/images/fruit_icon.png");
    fruit_icon.width *= 3;
    fruit_icon.height *= 3;
    seed_icon = LoadTexture("assets/images/seed_icon.png");
    seed_icon.width *= 3;
    seed_icon.height *= 3;

    customFont = LoadFontEx("assets/pixel_font.ttf", 48, nullptr, 0);

    tree_hit = LoadSound("assets/sounds/tree_hit.wav");
    bush_hit = LoadSound("assets/sounds/bush_hit.wav");
    block_sound = LoadSound("assets/sounds/block.mp3");

    pointer = LoadTexture("assets/images/pointer.png");
    home_button_tex = LoadTexture("assets/images/home_button.png");  
    home_button_tex.width *= 4;
    home_button_tex.height *= 4;

    std::cout << "Fimnished INnnitng!!\n";
}

bool data_loaded = false;
bool just_loaded = true;

void Main::Update()
{
    Player* player = game.GetEntityOfType<Player>();

    // Loads all data once
    if (!data_loaded)
    {
        if (player)
        {
            LoadData();
            data_loaded = true;
            just_loaded = true;
        }
    }

    UpdateTerrain();
    UpdateMusicStream(music);

    if (player != nullptr)
    {
        game.camera.target.x = player->x;
        game.camera.target.y = player->y;
        game.camera.offset = {(float)game.WIDTH/2, (float)game.HEIGHT/2};
    }

    RunEnemySpawningSystem();

    for (Plant* plant : game.GetEntitiesOfType<Plant>())
    {
        if (CheckCollisionRecs(*player->current_axe_hitbox,
            {(float)plant->x, (float)plant->y, (float)plant->texture.width*plant->scale, (float)plant->texture.height*plant->scale}
        ))
        {
            if (plant->type == PLANT_TYPE::TREE)
            {
                player->wood += 3;
                PlaySound(tree_hit);
            }
            else if (plant->type == PLANT_TYPE::BUSH)
            {
                player->wood++;
                player->fruit++;
                PlaySound(bush_hit);
            }
            plant->Delete();
        }
    } 

    just_loaded = false;
}

void Main::Draw()
{
    DrawTerrainAndPlants(noise, &entities,
        {(float)startX, (float)startY}, {(float)endX, (float)endY},
        tileAtlas, insidesAtlas,
        tree, bush, 2.5f,
        game.CELL_SIZE
    );

    Player* player = game.GetEntityOfType<Player>();
    if (!player) return;

    if (player->death_loot.available)
    {
        DrawCircleLines(player->death_loot.location.x, player->death_loot.location.y, 50, RED);

        int txt_w = MeasureText("X", 40);
        DrawText("X", (player->death_loot.location.x-txt_w/2), (player->death_loot.location.y-40/2), 40, RED);
    }

    // Debug lines
    // for (Plant* plant : game.GetEntitiesOfType<Plant>())
    // {
    //     DrawRectangleLines(plant->x, plant->y, plant->texture.width, plant->texture.height, RED);
    // }
}

void DrawStats()
{
    Player* player = game.GetEntityOfType<Player>();
    
    // wood stats
    DrawTexture(tree_icon, 20, 20, WHITE);
    DrawTextEx(customFont, std::to_string(player->wood).c_str(), { 80, 20 }, 48, 2, WHITE);

    // fruit stats
    DrawTexture(fruit_icon, 20, 70, WHITE);
    DrawTextEx(customFont, std::to_string(player->fruit).c_str(), { 80, 70 }, 48, 2, WHITE);

    // seeds stats
    DrawTexture(seed_icon, 20, 120, WHITE);
    DrawTextEx(customFont, std::to_string(player->seeds).c_str(), { 80, 120 }, 48, 2, WHITE);

    // std::string info_vel = "x: " + std::to_string(player->velocity.x) + "\ny:" + std::to_string(player->velocity.y);
    // DrawText(info_vel.c_str(), game.HEIGHT-200, 20, 20, BLACK);

    // Players HEALTH BAR
    DrawRectangleRounded({20, (float)game.HEIGHT-100, 300, 48}, 10.f, 100.f, WHITE);

    DrawRectangleRounded({
        20+4, (float)game.HEIGHT-97,
        (player->health / 100.f) * (300-8), 42
    },
    10.f, 100.f, RED);

    std::string health_str = std::to_string((int)player->health) + "%";
    DrawText(health_str.c_str(), 35, game.HEIGHT-87, 30, BLACK);
}

void DrawDeathPointer()
{
    Player* player = game.GetEntityOfType<Player>();
    if (!player) return;

    Vector2 pointer_pos = {(float)game.WIDTH-100, (float)game.HEIGHT-100};
    Vector2 death_pos = GetWorldToScreen2D(player->death_loot.location, game.camera);

    float rot = atan2f(death_pos.y - pointer_pos.y,
                       death_pos.x - pointer_pos.x) * RAD2DEG + 90.f;

    Rectangle src = { 0, 0, (float)pointer.width, (float)pointer.height };
    Rectangle dest = { (float)pointer_pos.x, (float)pointer_pos.y,
                       (float)pointer.width, (float)pointer.height };
    Vector2 origin = { pointer.width / 2.0f, pointer.height / 2.0f };

    DrawTexturePro(pointer, src, dest, origin, rot, WHITE);
}


void Main::DrawUI()
{
    DrawStats();
    DrawInventoryUI();

    Player* player = game.GetEntityOfType<Player>();
    if (player->death_loot.available)
    {
        DrawDeathPointer();
    }

    // Home button
    Rectangle home_button_rec = {
        (float)game.WIDTH - home_button_tex.width - 20, 20,
        (float)home_button_tex.width, (float)home_button_tex.height
    };

    if (CheckCollisionPointRec(GetMousePosition(), home_button_rec)
    && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        game.SetScene(Scenes::menu_scene.get());
    }
    DrawTexture(home_button_tex, home_button_rec.x, home_button_rec.y, WHITE);
}