#pragma once 

#include "gameInterface.hpp"

class SettingScene : public GameInterface
{
public:
    void renderScene() override;
    void handleEvent(const SDL_Event &e) override;
    void initScene() override;
};