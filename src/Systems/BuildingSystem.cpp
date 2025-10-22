#include "BuildingSystem.hpp"

#include <array>
#include <cmath>

#include "../Main/Main.hpp"
#include "../Game/Game.hpp"
#include "../Player/Player.hpp"
#include "../Block/Block.hpp"
#include "../Door/Door.hpp"
#include "../Crop/Crop.hpp"

float PLACE_BLOCK_RADIUS = 300.f;
float block_size = 50.f;
Color block_color;

enum BLOCK_TYPE
{
    BLOCK,
    DOOR
};
BLOCK_TYPE mode = BLOCK;

int wood_cost = 5;

std::array<BLOCK_TYPE, 2> mode_types = {BLOCK, DOOR};

extern Game game;
namespace Scenes
{
    extern std::unique_ptr<Main> main_scene;
}

void RunBuildingSystem()
{
    Player* player = game.GetEntityOfType<Player>();
    
    float snapped_x = std::floor(game.mouse_pos.x / block_size) * block_size;
    float snapped_y = std::floor(game.mouse_pos.y / block_size) * block_size;

    if (IsKeyPressed(KEY_LEFT)) { mode = mode_types[(mode + 1) % mode_types.size()]; }
    else if (IsKeyPressed(KEY_RIGHT)) { mode = mode_types[(mode - 1) % mode_types.size()]; }

    Block* block_over = nullptr;
    for (Block* block : game.GetEntitiesOfType<Block>())
    {
        if (CheckCollisionPointRec(game.mouse_pos, {(float)block->x, (float)block->y, (float)block->width, (float)block->height}))
        {
            block_over = block;
            break;
        }
    }

    bool over_player_or_plant_or_crop = false;
    
    if (CheckCollisionRecs({snapped_x, snapped_y, block_size, block_size}, player->hit_box))
        over_player_or_plant_or_crop = true;

    for (Plant* plant : game.GetEntitiesOfType<Plant>())
    {
        if (CheckCollisionRecs({snapped_x, snapped_y, block_size, block_size},
            {(float)plant->x, (float)plant->y, (float)plant->texture.width, (float)plant->texture.height}))
        {
            over_player_or_plant_or_crop = true;
        }
    }

    for (Crop* crop : game.GetEntitiesOfType<Crop>())
    {
        if (CheckCollisionRecs({snapped_x, snapped_y, block_size, block_size},
            {(float)crop->x, (float)crop->y, (float)crop->texture.width, (float)crop->texture.height}))
        {
            over_player_or_plant_or_crop = true;
        }
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointCircle(
        game.mouse_pos, {(float)player->x, (float)player->y}, PLACE_BLOCK_RADIUS
    )) // Makes sure its in the radius of the player so they dont try place it too far
    {
        // If user clicks over a place where a block already is, the block deletes and user gains the wood back.
        if (block_over != nullptr)
        {
            block_over->Delete();
            player->wood += wood_cost;

            PlaySound(Scenes::main_scene->block_sound);
        }
        // Otherwise a new block is added and the player loses wood.
        else if (!over_player_or_plant_or_crop
            && block_over == nullptr && (player->wood-wood_cost) >= 0)
        {
            switch (mode)
            {
            case BLOCK:
            {
                std::unique_ptr<Block> new_block = std::make_unique<Block>(snapped_x, snapped_y);
                game.AddEntity(std::move(new_block));
                break;
            }
            case DOOR:
            {
                std::unique_ptr<Door> new_door = std::make_unique<Door>(snapped_x, snapped_y);
                game.AddEntity(std::move(new_door));
                break;
            }
            default:
                break;
            }

            PlaySound(Scenes::main_scene->block_sound);
            player->wood -= wood_cost;
        }
    }

    if (!over_player_or_plant_or_crop && CheckCollisionPointCircle(
        game.mouse_pos, {(float)player->x, (float)player->y}, PLACE_BLOCK_RADIUS
    ))
    {
        // If its not over the player and is in radius
        block_color = (mode == BLOCK) ? Color{BROWN.r, BROWN.g, BROWN.b, 100} : Color{245, 222, 179, 100};
    }
    else
    {
        // If it is, it turns red to show its not allowed
        block_color = {RED.r, RED.g, RED.b, 100};
    }
}

void DrawBuildingSystem()
{
    float snapped_x = std::floor(game.mouse_pos.x / block_size) * block_size;
    float snapped_y = std::floor(game.mouse_pos.y / block_size) * block_size;
    DrawRectangleV({snapped_x, snapped_y}, {block_size, block_size}, block_color);

    // Draw block radius
    // Player* player = game.GetEntityOfType<Player>();
    // DrawCircleLines(player->x, player->y, 200.f, RED);
}