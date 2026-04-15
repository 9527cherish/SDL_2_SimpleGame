#pragma once 

#include "gameInterface.hpp"

class GameScene : public GameInterface
{
public:
    GameScene();
    ~GameScene() override;

    void renderScene() override;
    void renderCurrentPerson();
    void renderBackground(int cameraX, int cameraY);
    void renderRemotePersons(int cameraX, int cameraY, Uint32 deltaTime);

    void handleEvent(const SDL_Event &e) override;
    void initScene() override;
private:
    SDL_Renderer* m_pRenderer = nullptr;
    SDL_Texture* m_pBackgroundTexture = nullptr;
    int m_iBackgroundWidth = 0;
    int m_iBackgroundHeight = 0;
    Uint32 m_lastFrameTime = 0;
};
