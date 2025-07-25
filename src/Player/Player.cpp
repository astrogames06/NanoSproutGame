#include "Player.hpp"

#include <raymath.h>

const float PLR_SPEED = 5.f;
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
int framesSpeed = 8;
int frameRow = 0;

bool isAxeMode = false;

Rectangle axe_hit_boxes[4];

void Player::Init()
{
    spriteSheet = LoadTexture("assets/walking.png");
    frameWidth = spriteSheet.width / frameCount;
    frameHeight = spriteSheet.height / rowCount;
    frameRec = { 0.0f, 0.0f, (float)frameWidth, (float)frameHeight };

    axeSheet = LoadTexture("assets/axe.png");
    axeFrameWidth = axeSheet.width / axeFrameCount;
    axeFrameHeight = axeSheet.height / axeRowCount;
}
#include <iostream>
void Player::Update()
{
    axe_hit_boxes[0] = {(float)x-40, (float)y, 80, 50};
    axe_hit_boxes[1] = {(float)x-40, (float)y-75, 80, 50};
    axe_hit_boxes[2] = {(float)x, (float)y-40, 50, 80};
    axe_hit_boxes[3] = {(float)x-75, (float)y-40, 50, 80};

    Rectangle null_rec = {0,0,0,0};
    if (isAxeMode && currentFrame > 3) current_axe_hitbox = &axe_hit_boxes[frameRow];
    else current_axe_hitbox = &null_rec;

    std::cout << framesCounter << '\n';

    if (IsKeyPressed(KEY_E) && !isAxeMode)
    {
        isAxeMode = true;
        currentFrame = 0;
        framesCounter = 0;
    }

    if (IsKeyDown(KEY_W)) { y -= PLR_SPEED; frameRow = 1; }
    if (IsKeyDown(KEY_A)) { x -= PLR_SPEED; frameRow = 3; }
    if (IsKeyDown(KEY_S)) { y += PLR_SPEED; frameRow = 0; }
    if (IsKeyDown(KEY_D)) { x += PLR_SPEED; frameRow = 2; }

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
}

void Player::Draw()
{
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
        WHITE);
        //DrawRectangleLinesEx(axe_hit_boxes[frameRow], 1.f, GREEN);
        // DrawRectangleLines(
        //     x - (axeRec.width * PLR_TEXTURE_SCALE) / 2.0f,
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
        WHITE);
        // DrawRectangleLines(
        //     x - (frameRec.width * PLR_TEXTURE_SCALE) / 2.0f,
        //     y - (frameRec.height * PLR_TEXTURE_SCALE) / 2.0f,
        //     frameRec.width * PLR_TEXTURE_SCALE, 
        //     frameRec.height * PLR_TEXTURE_SCALE,
        // GREEN);
    }
}