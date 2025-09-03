#pragma once 

#include "gameInterface.hpp"

class SettingScene : public GameInterface
{
public:
    void renderScene() override;
    void handleEvent() override;
};