#include "Game.hpp"

#include <iostream>
#include <algorithm>
#include <raygui.h>

Texture2D nil_texture;

// Set up before raylib window is created
void Game::SetUp()
{
    WIDTH = GetScreenWidth();
    HEIGHT = GetScreenHeight();

    // Setting dimensions for window on web
    #if defined(PLATFORM_WEB)
        WIDTH = 1200;
        HEIGHT = 675;
    #endif
}

void Game::Init()
{
    #ifndef defined(PLATFORM_WEB)
        WIDTH = GetScreenWidth();
        HEIGHT = GetScreenHeight();
    #endif
    ToggleFullscreen();

    camera.target = { 0, 0 };
    camera.offset = { 0, 0 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    nil_texture = LoadTexture("assets/nil.png");
}
    
void Game::Update()
{
    mouse_pos = GetScreenToWorld2D(GetMousePosition(), camera);
    // Adds queued entities first
    while (!pending_entities.empty())
    {
        pending_entities.front()->Init();
        current_scene->entities.push_back(std::move(pending_entities.front()));
        pending_entities.pop();
    }

    current_scene->Update();
    for (std::unique_ptr<Entity>& entity : current_scene->entities)
    {
        entity->Update();
    }

    // Remove it if Entity->remove = true;
    current_scene->entities.erase(
        std::remove_if(
            current_scene->entities.begin(),
            current_scene->entities.end(),
            [](const std::unique_ptr<Entity>& e) { return e->remove; }
        ),
        current_scene->entities.end()
    );
}

void Game::Draw()
{
    BeginDrawing();
    ClearBackground(current_scene->background_color);
    BeginMode2D(camera);

    current_scene->Draw();
    for (std::unique_ptr<Entity>& entity : current_scene->entities)
    {
        entity->Draw();
    }

    EndMode2D();

    current_scene->DrawUI();
    EndDrawing();
}

void Game::Reset()
{

}

void Game::AddEntity(std::unique_ptr<Entity> entity)
{
    if (current_scene != nullptr)
    {
        entity->texture = nil_texture;
        entity->Init();
        pending_entities.push(std::move(entity));
    }
    else
        std::cout << "! WARNING CURRENT SCENE IS NULLPTR !\n";
}
void Game::SetScene(Scene* scene)
{
    current_scene = scene;
    current_scene->Init();
}