#ifndef SETTINGS_H
#define SETTINGS_H

#include "../Scene/Scene.hpp"

inline Color player_selected_color = WHITE;
inline float settings_music_volume = 10.f;
class Settings : public Scene
{
    void Init() override;
    void Update() override;
    void DrawUI() override;
};

#endif