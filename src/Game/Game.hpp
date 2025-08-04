#ifndef GAME_H
#define GAME_H

#include <vector>
#include <raylib.h>
#include <memory>
#include <string>
#include <queue>

#include "../Entity/Entity.hpp"
#include "../Scene/Scene.hpp"

class Game
{
public:
    int WIDTH;
    int HEIGHT;
    const std::string TITLE = "Nano Sprout";
    Vector2 mouse_pos;

    Camera2D camera;

    float CELL_SIZE = 35.f;
    
    void AddEntity(std::unique_ptr<Entity> entity);

    template <typename T>
    T* GetEntityOfType();

    template <typename T>
    std::vector<T*> GetEntitiesOfType();

    void SetScene(Scene* scene);

    void SetUp(); // Set up before raylib window is created
    void Init();
    void Update();
    void Draw();

    void Reset();
private:
    Scene* current_scene;
    std::queue<std::unique_ptr<Entity>> pending_entities;
};

template <typename T>
T* Game::GetEntityOfType()
{
    for (auto& entity : current_scene->entities)
    {
        T* result = dynamic_cast<T*>(entity.get());
        if (result != nullptr)
            return result;
    }
    return nullptr;
}

template <typename T>
std::vector<T*> Game::GetEntitiesOfType()
{
    std::vector<T*> results;
    for (auto& entity : current_scene->entities)
    {
        T* result = dynamic_cast<T*>(entity.get());
        if (result != nullptr)
            results.push_back(result);
    }
    return results;
}

#endif