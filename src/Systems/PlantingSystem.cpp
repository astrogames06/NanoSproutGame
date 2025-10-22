#include "PlantingSystem.hpp"

#include <raylib.h>
#include <memory>
#include <Terrain/Terrain.h>

#include "../Game/Game.hpp"
#include "../Player/Player.hpp"
#include "../Crop/Crop.hpp"

float cell_size = 50.f;
float PLACE_CROP_RADIUS = 300.f;
Color crop_color = WHITE;
Texture2D seed_tex;

extern Game game;

void InitPlantingSystem()
{
    seed_tex = LoadTexture("assets/images/crop/0.png");
}

void RunPlantingSystem()
{
    Player* player = game.GetEntityOfType<Player>();
    float snapped_x = std::floor(game.mouse_pos.x / cell_size) * cell_size;
    float snapped_y = std::floor(game.mouse_pos.y / cell_size) * cell_size;

    Crop* crop_over = nullptr;
    for (Crop* crop : game.GetEntitiesOfType<Crop>())
    {
        if (CheckCollisionPointRec(game.mouse_pos, {(float)crop->x, (float)crop->y, (float)cell_size, (float)cell_size}))
        {
            crop_over = crop;
            break;
        }
    }

    bool over_player_or_plant_or_block = false;
    
    if (CheckCollisionRecs({snapped_x, snapped_y, cell_size, cell_size}, player->hit_box))
        over_player_or_plant_or_block = true;

    for (Plant* plant : game.GetEntitiesOfType<Plant>())
    {
        if (CheckCollisionRecs({snapped_x, snapped_y, cell_size, cell_size},
            {(float)plant->x, (float)plant->y, (float)plant->texture.width, (float)plant->texture.height}))
        {
            over_player_or_plant_or_block = true;
        }
    }

    for (Block* block : game.GetEntitiesOfType<Block>())
    {
        if (CheckCollisionRecs({snapped_x, snapped_y, cell_size, cell_size},
            {(float)block->x, (float)block->y, (float)block->width, (float)block->height}))
        {
            over_player_or_plant_or_block = true;
        }
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointCircle(
        game.mouse_pos, {(float)player->x, (float)player->y}, PLACE_CROP_RADIUS
    )) // Makes sure its in the radius of the player so they dont try place it too far
    {
        // If user clicks over a place where a block already is, the block deletes and user gains the wood back.
        if (crop_over != nullptr)
        {
            crop_over->Delete();
            player->seeds++;

            // PlaySound(Scenes::main_scene->block_sound);
        }
        // Otherwise a new block is added and the player loses wood.
        else if (!over_player_or_plant_or_block
            && crop_over == nullptr && (player->seeds-1) >= 0)
        {
            std::unique_ptr<Crop> new_crop = std::make_unique<Crop>();
            new_crop->x = snapped_x;
            new_crop->y = snapped_y;
            game.AddEntity(std::move(new_crop));

            // PlaySound(Scenes::main_scene->block_sound);
            player->seeds--;
        }
    }

    if (!over_player_or_plant_or_block && CheckCollisionPointCircle(
        game.mouse_pos, {(float)player->x, (float)player->y}, PLACE_CROP_RADIUS
    ))
    {
        // If its not over the player and is in radius
        crop_color = WHITE;
    }
    else
    {
        // If it is, it turns red to show its not allowed
        crop_color = RED;
    }
}

void DrawPlantingSystem()
{
    Player* player = game.GetEntityOfType<Player>();
    float snapped_x = std::floor(game.mouse_pos.x / cell_size) * cell_size;
    float snapped_y = std::floor(game.mouse_pos.y / cell_size) * cell_size;

    DrawRectangle(snapped_x, snapped_y, cell_size, cell_size, Color{crop_color.r, crop_color.g, crop_color.b, 100});
    DrawTexture(seed_tex, snapped_x, snapped_y, crop_color);
}