#include "Main.hpp"

#include "TerrainSetup.hpp"

#include "../Systems/InventorySystem.hpp"

Font customFont;
Texture2D tree_icon;
Texture2D fruit_icon;

Texture2D inventory_tex;

void Main::Init()
{
    game.CELL_SIZE = 64;
    background_color = Color{155, 212, 195, 255};
    std::cout << "Main Scene initialized\n";
    std::unique_ptr<Player> player = std::make_unique<Player>(100, 100);
    game.AddEntity(std::move(player));

    SetUpTerrain();

    inventory_tex = LoadTexture("assets/inventory.png");
    inventory_tex.width *= 1.5;
    inventory_tex.height *= 1.5;

    tree_icon = LoadTexture("assets/wood_icon.png");
    tree_icon.width *= 3;
    tree_icon.height *= 3;
    fruit_icon = LoadTexture("assets/fruit_icon.png");
    fruit_icon.width *= 3;
    fruit_icon.height *= 3;

    customFont = LoadFontEx("assets/pixel_font.ttf", 48, nullptr, 0);
}

void Main::Update()
{
    UpdateTerrain();

    Player* player = game.GetEntityOfType<Player>();
    game.camera.target.x = player->x;
    game.camera.target.y = player->y;
    game.camera.offset = {(float)game.WIDTH/2, (float)game.HEIGHT/2};

    if (IsKeyPressed(KEY_O))
    {
        std::unique_ptr<Enemy> new_enemy = std::make_unique<Enemy>(0, 0);
        new_enemy->x = GetRandomValue(0, game.WIDTH);
        new_enemy->y = GetRandomValue(0, game.HEIGHT);
        game.AddEntity(std::move(new_enemy));
    }

    for (Plant* plant : game.GetEntitiesOfType<Plant>())
    {
        // if (!plant->remove)
        //     std::cout << "Plant at " << plant->x << ", " << plant->y << " is alive\n";
        // else
        //     std::cout << "Plant at " << plant->x << ", " << plant->y << " marked for removal\n";

        if (CheckCollisionRecs(*player->current_axe_hitbox,
            {(float)plant->x, (float)plant->y, (float)plant->texture.width*plant->scale, (float)plant->texture.height*plant->scale}
        ))
        {
            // std::cout << "Deleting plant at " << plant->x << ", " << plant->y << "\n";
            if (plant->type == PLANT_TYPE::TREE)
            {
                player->wood += 3;
            }
            else if (plant->type == PLANT_TYPE::BUSH)
            {
                player->wood++;
                player->fruit++;
            }
            plant->Delete();
        }
    }
}

void Main::Draw()
{
    DrawTerrainAndPlants(noise, &entities,
        {(float)startX, (float)startY}, {(float)endX, (float)endY},
        tileAtlas, insidesAtlas,
        tree, bush, 2.5f,
        game.CELL_SIZE
    );
}

void DrawStats()
{
    Player* player = game.GetEntityOfType<Player>();
    
    DrawTexture(tree_icon, 20, 20, WHITE);
    DrawTextEx(customFont, std::to_string(player->wood).c_str(), { 80, 20 }, 48, 2, WHITE);

    DrawTexture(fruit_icon, 20, 70, WHITE);
    DrawTextEx(customFont, std::to_string(player->fruit).c_str(), { 80, 70 }, 48, 2, WHITE);

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

void Main::DrawUI()
{
    DrawStats();
    DrawInventoryUI(inventory_tex);
}