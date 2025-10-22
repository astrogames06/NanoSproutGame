#include "Crop.hpp"

#include <string>
#include <vector>

#include "../Player/Player.hpp"

std::vector<Texture2D> crop_textures;

void Crop::Init()
{
    // Sets starting time
    last_time = std::chrono::high_resolution_clock::now();

    texture = LoadTexture("assets/images/crop/0.png");
    for (int i = 0; i < 4; i++)
    {
        std::string tex_str = "assets/images/crop/" + std::to_string(i) + ".png";
        crop_textures.push_back(LoadTexture(tex_str.c_str()));
    }
}

void Crop::Update()
{
    // Makes sure it doesnt grow further once its fully grown
    if (points <= 180)
    {
        // Gets current time
        std::chrono::high_resolution_clock::time_point now_time = std::chrono::high_resolution_clock::now();
        // Gets the distance between the starting time and current time
        std::chrono::seconds elapsed_time = std::chrono::duration_cast<std::chrono::seconds>(now_time - last_time);

        // Checks if the distance is one second
        if (elapsed_time.count() >= 1)
        {
            // Adds point (second)
            points++;

            // Resets the starting time
            last_time = now_time;
        }
    }

    // Sets texture based on state
    if (points < 60)
    {
        texture = crop_textures[0];
    }
    else if (points > 60 && points < 120)
    {
        texture = crop_textures[1];
    }
    else if (points > 120 && points < 180)
    {
        texture = crop_textures[2];
    }
    else if (points > 180)
    {
        texture = crop_textures[3];
    }

    // Harvesting the crop
    Player* player = game.GetEntityOfType<Player>();

    if (CheckCollisionRecs(*player->current_axe_hitbox,
        {(float)x, (float)y, (float)texture.width, (float)texture.height}
    ) && points >= 180) // Checks if player axe hits it and its fully grown
    {
        // Gives player fruit
        player->fruit += 3;

        // Sets the crop back to the stage right before having fruit
        points -= 60;
    } 
}

void Crop::Draw()
{
    DrawTexture(texture, x, y, WHITE);

    Color points_color = RED;
    if (points >= 180) points_color = GREEN; // Lets the player know its ready to harvest
    DrawText(std::to_string(points).c_str(), x, y, 10, RED);
}