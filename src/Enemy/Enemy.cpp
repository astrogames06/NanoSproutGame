#include "Enemy.hpp"

#include <raymath.h>

#include "../Player/Player.hpp"

bool follow_player = false;

Vector2 velocity;
float acceleration = 1.f;
float speed = 3.f;

void Enemy::Init()
{

}

void Enemy::Update()
{
    x += velocity.x;
    y += velocity.y;
    velocity = Vector2Scale(velocity, 0.9f);

    Player* player = game.GetEntityOfType<Player>();
    if (CheckCollisionCircleRec(
        {(float)x, (float)y}, game.CELL_SIZE*5,
        {(float)player->x, (float)player->y, game.CELL_SIZE, game.CELL_SIZE}
    ))
    {
        follow_player = true;
    }

    if (follow_player && player != nullptr)
    {
        // Vector2 toPlayer = Vector2Subtract({player->x, player->y}, {x, y});
        // if (Vector2Length(toPlayer) > 1.0f) {
        //     Vector2 dir = Vector2Normalize(toPlayer);
        //     velocity = Vector2Add(velocity, Vector2Scale(dir, acceleration));
        //     if (Vector2Length(velocity) > speed)
        //         velocity = Vector2Scale(Vector2Normalize(velocity), speed);
        // }
    }

    if (CheckCollisionRecs(
        {(float)x, (float)y, game.CELL_SIZE, game.CELL_SIZE},
        *player->current_axe_hitbox
    ))
    {
        // Vector2 dir = Vector2Normalize(Vector2Subtract(
        //     {(float)player->x, (float)player->y}, {(float)x, (float)y}));
        // velocity = Vector2Negate(Vector2Add(velocity, Vector2Scale(dir, acceleration)));
        Delete();
    }

    // DrawRectangleRec(*player->current_axe_hitbox, BLUE);
}

void Enemy::Draw()
{
    DrawRectangleRec(
        {(float)x, (float)y, game.CELL_SIZE, game.CELL_SIZE},
    RED);
    // DrawCircleLines(x, y, game.CELL_SIZE*5, GREEN);
}