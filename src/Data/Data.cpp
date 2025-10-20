#include "Data.hpp"

#include <fstream>
#include <memory>
#include <nlohmann/json.hpp>

#include "../Game/Game.hpp"
#include "../Main/Main.hpp"
#include "../Menu/Menu.hpp"

#include "../Block/Block.hpp"
#include "../Door/Door.hpp"

namespace Scenes
{
    extern std::unique_ptr<Main> main_scene;
    extern std::unique_ptr<Menu> menu_scene;
}

void SaveData()
{
    nlohmann::ordered_json json_save;

    // Save players position and stats
    Player* player = game.GetEntityOfType<Player>();

    json_save["player"]["x"] = player->x;
    json_save["player"]["y"] = player->y;

    json_save["player"]["wood"] = player->wood;
    json_save["player"]["fruit"] = player->fruit;

    // Saving blocks
    json_save["blocks"] = nlohmann::json::array();
    for (Block* block : game.GetEntitiesOfType<Block>())
    {
        bool is_door = false;
        if (dynamic_cast<Door*>(block) != nullptr)
            is_door = true;

        json_save["blocks"].push_back({
            {"x", block->x},
            {"y", block->y},
            {"is_door", is_door}
        });
    }

    // Writing the data to save.json
    std::ofstream file("save.json");
    if (file.is_open())
        file << json_save.dump(4);
    else
        std::cerr << "FAILED TO SAVE TO SAVE.JSON!\n";
}

void LoadData()
{
    // Loading file and json
    std::ifstream file("save.json");
    if (!file.is_open())
    {
        std::cerr << "Failed to load save.json\n";
        return;
    }
    nlohmann::json json_load;
    file >> json_load;

    // Loads player position and stats
    Player* player = game.GetEntityOfType<Player>();

    player->x = json_load["player"]["x"].get<float>();
    player->y = json_load["player"]["y"].get<float>();

    player->wood = json_load["player"]["wood"].get<float>();
    player->fruit = json_load["player"]["fruit"].get<float>();

    // Loading blocks
    for (nlohmann::json& block : json_load["blocks"])
    {
        if (block["is_door"].get<bool>())
        {
            std::unique_ptr<Door> loaded_door = std::make_unique<Door>(
                block["x"].get<float>(), block["y"].get<float>()
            );
            game.AddEntity(std::move(loaded_door));
        }
        else
        {
            std::unique_ptr<Block> loaded_block = std::make_unique<Block>(
                block["x"].get<float>(), block["y"].get<float>()
            );
            game.AddEntity(std::move(loaded_block));
        }
    }
}