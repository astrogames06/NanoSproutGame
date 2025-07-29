#include "Enemy.hpp"

#include <raymath.h>
#include <iostream>

#include "../Player/Player.hpp"

void Enemy::Init()
{

}

void Enemy::Update()
{
    velocity = Vector2Scale(velocity, 0.9f);

    Player* player = game.GetEntityOfType<Player>();
    if (CheckCollisionCircleRec(
        {(float)x, (float)y}, game.CELL_SIZE*5,
        player->hit_box
    ))
    {
        follow_player = true;
    } else follow_player = false;

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

    if (CheckCollisionRecs({(float)x, (float)y, game.CELL_SIZE, game.CELL_SIZE}, player->hit_box))
    {
        std::cout << "dir: " << player->axe_direction << '\n';
        Vector2 enemy_pos = {(float)x, (float)y};
        Vector2 player_pos = {(float)player->x, (float)player->y};


        Vector2 push_back = Vector2Normalize(Vector2Subtract(player_pos, enemy_pos));
        float strength = 10000.f;

        player->velocity = Vector2Add(player->velocity, Vector2Scale(push_back, strength * GetFrameTime()));
    }

    if (CheckCollisionRecs(
        {(float)x, (float)y, game.CELL_SIZE, game.CELL_SIZE},
        *player->current_axe_hitbox
    ))
    {
        std::cout << "dir: " << player->axe_direction << '\n';
        velocity = {0, 0};
        Vector2 push_back = {0, 0};
        float strength = 100.f;

        switch (player->axe_direction)
        {
            case AXE_DIRECTION::LEFT:  push_back = {-strength, 0}; break;
            case AXE_DIRECTION::RIGHT: push_back = { strength, 0}; break;
            case AXE_DIRECTION::UP:    push_back = { 0, -strength}; break;
            case AXE_DIRECTION::DOWN:  push_back = { 0,  strength}; break;
        }
        std::cout << push_back.x << " " << push_back.y << '\n';

        velocity = Vector2Add(velocity, push_back);

        health -= 10;
    }

    // Removes itself if health is 0 (or below)
    if (health <= 0) Delete();

    // DrawRectangleRec(*player->current_axe_hitbox, BLUE);
    x += velocity.x;
    y += velocity.y;
}

void Enemy::Draw()
{
    DrawRectangleRec(
        {(float)x, (float)y, game.CELL_SIZE, game.CELL_SIZE},
    RED);
    // DrawCircleLinesV({(float)x, (float)y}, game.CELL_SIZE*5, BLACK);

    if (health < 100)
    {
        DrawRectangle(
            x-4,
            y-30,
            game.CELL_SIZE + 8,
            20,
        WHITE);
        DrawRectangle(
            x-2,
            y-28,
            (health / 100.f) * (game.CELL_SIZE + 4),
            16,
        RED);
    }
    // DrawCircleLines(x, y, game.CELL_SIZE*5, GREEN);
}