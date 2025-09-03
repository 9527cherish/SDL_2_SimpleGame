#pragma once 

#include "gameInterface.hpp"

class GameScene : public GameInterface
{
public:
    void renderScene() override;
    void handleEvent() override;

};