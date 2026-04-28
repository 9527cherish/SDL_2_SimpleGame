#pragma once 

#include "gameInterface.hpp"
#include "camera.hpp"

class GameScene : public GameInterface
{
public:
    GameScene();
    ~GameScene() override;

    void renderScene() override;
    void renderCurrentPerson();
    void renderBackground();
    void renderRemotePersons(Uint32 deltaTime);

    void handleEvent(const SDL_Event &e) override;
    void initScene() override;
private:
    SDL_Renderer* m_pRenderer = nullptr;
    SDL_Texture* m_pBackgroundTexture = nullptr;
    int m_iBackgroundWidth = 0;
    int m_iBackgroundHeight = 0;
    Camera m_camera;
    Uint32 m_lastFrameTime = 0;
};
