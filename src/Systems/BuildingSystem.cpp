#include "BuildingSystem.hpp"

#include "../Game/Game.hpp"
#include "../Player/Player.hpp"
#include "../Block/Block.hpp"
#include "../Door/Door.hpp"

float PLACE_BLOCK_RADIUS = 300.f;
float block_size = 50.f;
Color block_color;

enum BLOCK_TYPE
{
    BLOCK,
    DOOR
};
BLOCK_TYPE mode = BLOCK;

extern Game game;

void RunBuildingSystem()
{
    Player* player = game.GetEntityOfType<Player>();
    float snapped_x = std::floor(game.mouse_pos.x / block_size) * block_size;
    float snapped_y = std::floor(game.mouse_pos.y / block_size) * block_size;

    if (IsKeyPressed(KEY_LEFT)) {mode=BLOCK;}
    else if (IsKeyPressed(KEY_RIGHT)) {mode=DOOR;}

    Block* block_over = nullptr;
    for (Block* block : game.GetEntitiesOfType<Block>())
    {
        if (CheckCollisionPointRec(game.mouse_pos, {(float)block->x, (float)block->y, (float)block->width, (float)block->height}))
        {
            block_over = block;
            break;
        }
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointCircle(
        game.mouse_pos, {(float)player->x, (float)player->y}, PLACE_BLOCK_RADIUS
    ))
    {
        if (block_over != nullptr)
        {
            block_over->Delete();
        }
        else if (!CheckCollisionRecs({snapped_x, snapped_y, block_size, block_size}, player->hit_box)
            && block_over == nullptr)
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
        }
    }

    if (!CheckCollisionRecs({snapped_x, snapped_y, block_size, block_size}, player->hit_box)
    && CheckCollisionPointCircle(
        game.mouse_pos, {(float)player->x, (float)player->y}, PLACE_BLOCK_RADIUS
    ))
    {
        // If its not over the player and is in radius
        block_color = {BROWN.r, BROWN.g, BROWN.b, 100};
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