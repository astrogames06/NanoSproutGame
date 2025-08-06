#include "Enemy.hpp"

#include <raymath.h>
#include <iostream>

#include "Terrain/Terrain.h"

#include "../Player/Player.hpp"
#include "../Main/Main.hpp"
#include "../Block/Block.hpp"

float RADIUS_TO_ATTACK;
float RADIUS_TO_FOLLOW;

Texture2D left_tex;
Texture2D right_tex;

namespace Scenes {
    extern std::unique_ptr<Main> main_scene;
}

void Enemy::Init()
{
    RADIUS_TO_ATTACK = game.CELL_SIZE*5;
    RADIUS_TO_FOLLOW = game.CELL_SIZE*10;

    left_tex = LoadTexture("assets/angry_chicken_left.png");
    left_tex.width *= 3;
    left_tex.height *= 3;

    right_tex = LoadTexture("assets/angry_chicken_right.png");
    right_tex.width *= 3;
    right_tex.height *= 3;

    // Sets initial texture
    texture = left_tex;
}

void CheckCollisions(Enemy &enemy, bool horizontal)
{
    for (Block* block : game.GetEntitiesOfType<Block>())
    {
        Rectangle blockRect = { (float)block->x, (float)block->y, (float)block->width, (float)block->height };
        Rectangle hit_box = {
            (float)enemy.x,
            (float)enemy.y,
            (float)enemy.texture.width,
            (float)enemy.texture.height
        };
        if (CheckCollisionRecs(hit_box, blockRect))
        {
            if (horizontal)
            {
                if (enemy.velocity.x > 0)
                {
                    enemy.x = blockRect.x - hit_box.width;
                    enemy.velocity.x = 0;
                }
                else if (enemy.velocity.x < 0)
                {
                    enemy.x = blockRect.x + blockRect.width;
                    enemy.velocity.x = 0;
                }
            }
            else
            {
                if (enemy.velocity.y > 0)
                {
                    enemy.y = blockRect.y - hit_box.height;
                    enemy.velocity.y = 0;
                }
                else if (enemy.velocity.y < 0)
                {
                    enemy.y = blockRect.y + blockRect.height;
                    enemy.velocity.y = 0;
                }
            }
        }
    }
}

void Enemy::Update()
{
    velocity = Vector2Scale(velocity, 0.9f);

    texture = (velocity.x > 0.1f) ? left_tex : (velocity.x < -0.1f ? right_tex : texture);


    Player* player = game.GetEntityOfType<Player>();

    // Checks if player gets close to enemy, if so it follows it
    if (CheckCollisionCircleRec(
        {(float)x, (float)y}, RADIUS_TO_ATTACK,
        player->hit_box
    ))
    {
        follow_player = true;
    }
    // If the player runs away to a certain distance the enemy will stop following it
    else if (!CheckCollisionCircleRec(
        {(float)x, (float)y}, RADIUS_TO_FOLLOW,
        player->hit_box
    ))
    {
        follow_player = false;
    }

    // If player goes in water, the enemy will stop following him
    if (!IsOnLand(player->rect, Scenes::main_scene->noise, game.CELL_SIZE))
    {
        follow_player = false;
    }

    // Makes sure enemy stands still if not following player
    if (!follow_player)
    {
        velocity = {0, 0};
    }

    if (follow_player && player != nullptr)
    {
        Vector2 enemy_pos = {(float)x, (float)y};
        Vector2 player_pos = {(float)player->x, (float)player->y};

        Vector2 dir = Vector2Normalize(Vector2Subtract(player_pos, enemy_pos));
        velocity = Vector2Add(velocity, Vector2Scale(dir, acceleration));

        // Clamp velocity to max speed
        if (Vector2Length(velocity) > speed)
        {
            velocity = Vector2Scale(Vector2Normalize(velocity), speed);
        }
    }

    // If player gets hit my enemy
    if (CheckCollisionRecs({(float)x, (float)y, (float)texture.width, (float)texture.height}, player->hit_box))
    {
        std::cout << "dir: " << player->direction << '\n';
        Vector2 enemy_pos = {(float)x, (float)y};
        Vector2 player_pos = {(float)player->x, (float)player->y};

        Vector2 push_back = Vector2Normalize(Vector2Subtract(player_pos, enemy_pos));
        float strength = 10000.f;

        player->velocity = Vector2Add(player->velocity, Vector2Scale(push_back, strength * GetFrameTime()));
        player->health -= 5;     
    }

    // If enemy gets hit by player
    if (CheckCollisionRecs(
        {(float)x, (float)y, game.CELL_SIZE, game.CELL_SIZE},
        *player->current_axe_hitbox
    ))
    {
        std::cout << "dir: " << player->direction << '\n';
        velocity = {0, 0};
        Vector2 push_back = {0, 0};
        float strength = 100.f;

        switch (player->direction)
        {
            case DIRECTION::LEFT:  push_back = {-strength, 0}; break;
            case DIRECTION::RIGHT: push_back = { strength, 0}; break;
            case DIRECTION::UP:    push_back = { 0, -strength}; break;
            case DIRECTION::DOWN:  push_back = { 0,  strength}; break;
        }
        std::cout << push_back.x << " " << push_back.y << '\n';

        velocity = Vector2Add(velocity, push_back);

        health -= 10;
    }

    // Removes itself if health is 0 (or below)
    if (health <= 0) Delete();

    // DrawRectangleRec(*player->current_axe_hitbox, BLUE);
    x += velocity.x;
    CheckCollisions(*this, true);
    y += velocity.y;
    CheckCollisions(*this, false);
}

void Enemy::Draw()
{
    DrawTexture(texture,
        (float)x, (float)y,
    WHITE);
    // DrawRectangleLinesEx({
    //     (float)x,
    //     (float)y,
    //     (float)texture.width,
    //     (float)texture.height
    // }, 1.f, GREEN);

    // Draws enemy health bar
    if (health < 100)
    {
        DrawRectangle(
            x-texture.width/2,
            y-30,
            game.CELL_SIZE + 8,
            20,
        WHITE);
        DrawRectangle(
            x-texture.width/2+2,
            y-28,
            (health / 100.f) * (game.CELL_SIZE + 4),
            16,
        RED);
    }

    // DEBUG LINES
    // DrawCircleLines(x, y, RADIUS_TO_ATTACK, RED);
    // DrawCircleLines(x, y, RADIUS_TO_FOLLOW, ORANGE);
}