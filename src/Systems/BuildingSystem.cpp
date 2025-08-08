#include "BuildingSystem.hpp"

#include <array>
#include <cmath>

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

int wood_cost = 5;

std::array<BLOCK_TYPE, 2> mode_types = {BLOCK, DOOR};

extern Game game;

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

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointCircle(
        game.mouse_pos, {(float)player->x, (float)player->y}, PLACE_BLOCK_RADIUS
    ))
    {
        // If user clicks over a place where a block already is, the block deletes and user gains the wood back.
        if (block_over != nullptr)
        {
            block_over->Delete();
            player->wood += wood_cost;
        }
        // Otherwise a new block is added and the player loses wood.
        else if (!CheckCollisionRecs({snapped_x, snapped_y, block_size, block_size}, player->hit_box)
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

            player->wood -= wood_cost;
        }
    }

    if (!CheckCollisionRecs({snapped_x, snapped_y, block_size, block_size}, player->hit_box)
    && CheckCollisionPointCircle(
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