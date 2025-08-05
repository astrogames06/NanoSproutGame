#include "Player.hpp"

#include <raymath.h>

#include "Terrain/Terrain.h"
#include "../Block/Block.hpp"
#include "../Door/Door.hpp"

#include "../Systems/BuildingSystem.hpp"

const float PLR_SPEED = 400.f;
const float PLR_TEXTURE_SCALE = 5.f;

Texture2D spriteSheet;
int frameCount = 8;
int rowCount = 4;
int frameWidth;
int frameHeight;

Rectangle axeRec;
Texture2D axeSheet;
int axeFrameCount = 8;
int axeRowCount = 4;
int axeFrameWidth;
int axeFrameHeight;

Rectangle frameRec;
int currentFrame = 0;
int framesCounter = 0;
int framesSpeed = 10;
int frameRow = 0;

bool isAxeMode = false;

Rectangle axe_hit_boxes[4];

#include "../Main/Main.hpp"
namespace Scenes {
    extern std::unique_ptr<Main> main_scene;
}
void Player::Init()
{
    spriteSheet = LoadTexture("assets/walking.png");
    frameWidth = spriteSheet.width / frameCount;
    frameHeight = spriteSheet.height / rowCount;
    frameRec = { 0.0f, 0.0f, (float)frameWidth, (float)frameHeight };

    axeSheet = LoadTexture("assets/axe.png");
    axeFrameWidth = axeSheet.width / axeFrameCount;
    axeFrameHeight = axeSheet.height / axeRowCount;

    air = 100.f;
    health = 100.f;
}

void CheckCollisions(Player &player, bool horizontal)
{
    for (Block* block : game.GetEntitiesOfType<Block>())
    {
        if (dynamic_cast<Door*>(block) == nullptr)
        {
            Rectangle blockRect = { (float)block->x, (float)block->y, (float)block->width, (float)block->height };

            if (CheckCollisionRecs(player.hit_box, blockRect))
            {
                if (horizontal)
                {
                    if (player.velocity.x > 0)
                    {
                        player.x = blockRect.x - player.hit_box.width / 2.0f;
                        player.velocity.x = 0;
                    }
                    else if (player.velocity.x < 0)
                    {
                        player.x = blockRect.x + blockRect.width + player.hit_box.width / 2.0f;
                        player.velocity.x = 0;
                    }
                }
                else
                {
                    if (player.velocity.y > 0)
                    {
                        player.y = blockRect.y - player.hit_box.height / 2.0f;
                        player.velocity.y = 0;
                    }
                    else if (player.velocity.y < 0)
                    {
                        player.y = blockRect.y + blockRect.height + player.hit_box.height / 2.0f;
                        player.velocity.y = 0;
                    }
                }

                player.hit_box = { player.x - game.CELL_SIZE/2, player.y - game.CELL_SIZE/2, game.CELL_SIZE, game.CELL_SIZE };
            }
        }
    }
}

void Player::Update()
{
    velocity = Vector2Scale(velocity, 0.3f);
    // makes sure velocity doesnt go too high
    velocity = Vector2Clamp(velocity, {-30.f, -30.f}, {30.f, 30.f});

    rect = {
        x - (spriteSheet.width * PLR_TEXTURE_SCALE) / 2.0f,
        y - (spriteSheet.height * PLR_TEXTURE_SCALE) / 2.0f,
        spriteSheet.width * PLR_TEXTURE_SCALE,
        spriteSheet.height * PLR_TEXTURE_SCALE
    };

    axe_hit_boxes[0] = {(float)x-40, (float)y, 80, 50};
    axe_hit_boxes[1] = {(float)x-40, (float)y-75, 80, 50};
    axe_hit_boxes[2] = {(float)x, (float)y-40, 50, 80};
    axe_hit_boxes[3] = {(float)x-75, (float)y-40, 50, 80};

    Rectangle null_rec = {NULL, NULL, NULL, NULL};
    if (isAxeMode && currentFrame > 3) current_axe_hitbox = &axe_hit_boxes[frameRow];
    else current_axe_hitbox = &null_rec;

    RunBuildingSystem();

    if (IsKeyPressed(KEY_E) && !isAxeMode)
    {
        isAxeMode = true;
        currentFrame = 0;
        framesCounter = 0;
    }

    if (IsKeyDown(KEY_W))
    { 
        velocity.y -= PLR_SPEED * GetFrameTime();
        frameRow = 1; direction = DIRECTION::UP;
    }
    if (IsKeyDown(KEY_A))
    {
        velocity.x -= PLR_SPEED * GetFrameTime();
        frameRow = 3; direction = DIRECTION::LEFT;
    }
    if (IsKeyDown(KEY_S))
    {
        velocity.y += PLR_SPEED * GetFrameTime();
        frameRow = 0; direction = DIRECTION::DOWN;
    }
    if (IsKeyDown(KEY_D))
    {
        velocity.x += PLR_SPEED * GetFrameTime();
        frameRow = 2; direction = DIRECTION::RIGHT;
    }

    
    if (!IsOnLand(rect, Scenes::main_scene->noise, game.CELL_SIZE))
    {
        tint = Color{155, 212, 195, 100};
        air -= 10.f * GetFrameTime();
    } else {
        tint = WHITE;
        air = 100.f;
    }

    if (isAxeMode)
    {
        // Loading animation
        framesCounter++;
		if (currentFrame >= axeFrameCount)
		{
			isAxeMode = false;
		}
        if (framesCounter >= (60 / (framesSpeed*1.5))) {
            framesCounter = 0;
            currentFrame = (currentFrame + 1);
        }
		axeRec = {
            (float)(currentFrame * axeFrameWidth),
            0.0f,
            (float)axeFrameWidth,
            (float)axeFrameHeight
        };
		axeRec.y = axeFrameHeight * frameRow;
    }
    else
    {
        frameRec.y = frameHeight * frameRow;

        if (IsKeyDown(KEY_W) || IsKeyDown(KEY_A) || IsKeyDown(KEY_S) || IsKeyDown(KEY_D)) {
            framesCounter++;
        }
        else {
            framesCounter = 0;
            currentFrame = 0;
            frameRec.x = (float)currentFrame * frameWidth;
        }

        if (framesCounter >= (60 / framesSpeed)) {
            framesCounter = 0;
            currentFrame = (currentFrame + 1) % frameCount;
            frameRec.x = (float)currentFrame * frameWidth;
        }
    }

    x += velocity.x;
    hit_box = { x - (frameRec.width * PLR_TEXTURE_SCALE)/2, y - (frameRec.height * PLR_TEXTURE_SCALE)/2,
                frameWidth * PLR_TEXTURE_SCALE, frameHeight * PLR_TEXTURE_SCALE };
    CheckCollisions(*this, true);

    y += velocity.y;
    hit_box = { x - (frameRec.width * PLR_TEXTURE_SCALE)/2, y - (frameRec.height * PLR_TEXTURE_SCALE)/2,
                frameWidth * PLR_TEXTURE_SCALE, frameHeight * PLR_TEXTURE_SCALE };
    CheckCollisions(*this, false);
}

void Player::Draw()
{
    DrawBuildingSystem();
    if (isAxeMode)
    {
        DrawTexturePro(axeSheet,
            axeRec,
            {
                x - (axeRec.width * PLR_TEXTURE_SCALE) / 2.0f,
                y - (axeRec.height * PLR_TEXTURE_SCALE) / 2.0f,
                axeRec.width * PLR_TEXTURE_SCALE,
                axeRec.height * PLR_TEXTURE_SCALE
            },
            { 0, 0 },
            0.f,
        tint);

        // DrawRectangleLinesEx(axe_hit_boxes[frameRow], 1.f, GREEN);
        // DrawRectangleLines(
        //     x - (axeRecidth * PLR_TEXTURE_SCALE) / 2.0f,
        //     y - (axeRec.height * PLR_TEXTURE_SCALE) / 2.0f,
        //     axeRec.width * PLR_TEXTURE_SCALE, 
        //     axeRec.height * PLR_TEXTURE_SCALE,
        // GREEN);
    }
    else
    {
        DrawTexturePro(spriteSheet,
            frameRec,
            {
                x - (frameRec.width * PLR_TEXTURE_SCALE) / 2.0f,
                y - (frameRec.height * PLR_TEXTURE_SCALE) / 2.0f,
                frameRec.width * PLR_TEXTURE_SCALE,
                frameRec.height * PLR_TEXTURE_SCALE
            },
            { 0, 0 },
            0.f,
        tint);
        // DrawRectangleLines(
        //     x - (frameRec.width * PLR_TEXTURE_SCALE) / 2.0f,
        //     y - (frameRec.height * PLR_TEXTURE_SCALE) / 2.0f,
        //     frameRec.width * PLR_TEXTURE_SCALE, 
        //     frameRec.height * PLR_TEXTURE_SCALE,
        // GREEN);
    }
    // DrawRectangleLinesEx(hit_box, 1.f, BLACK);
    // DrawRectangleLinesEx(rect, 1.f, BLUE);

    // Air bar (air / maxHealth) * barMaxWidth;
    if (!IsOnLand(rect, Scenes::main_scene->noise, game.CELL_SIZE))
    {
        DrawRectangle(
            x-(frameRec.width * PLR_TEXTURE_SCALE) / 2.0f,
            y-game.CELL_SIZE-10, frameRec.width * PLR_TEXTURE_SCALE,
            20,
        WHITE);
        DrawRectangle(
            x-(frameRec.width * PLR_TEXTURE_SCALE) / 2.0f + 2,
            y-game.CELL_SIZE-8,
            (air / 100.f) * (frameRec.width * PLR_TEXTURE_SCALE - 4),
            16,
        BLUE);
    }
}