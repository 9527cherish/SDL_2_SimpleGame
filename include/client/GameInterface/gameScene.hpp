#pragma once 

#include "gameInterface.hpp"

class GameScene : public GameInterface
{
public:
    void renderScene() override;
    void renderCurrentPerson();


    void handleEvent(const SDL_Event &e) override;
    void initScene() override;
private:
    SDL_Renderer* m_pRenderer;
    Uint32 m_lastFrameTime = 0;
};
