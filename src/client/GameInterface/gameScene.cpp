#include "gameScene.hpp"
#include <algorithm>
#include <SDL2/SDL_image.h>
#include <spdlog/spdlog.h>
#include "persona.hpp"
#include "interfaceManager.hpp"
#include "dataManager.hpp"
#include "netClient.hpp"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
    if (m_pBackgroundTexture != nullptr)
    {
        SDL_DestroyTexture(m_pBackgroundTexture);
        m_pBackgroundTexture = nullptr;
    }
}

void GameScene::renderScene()
{
    Uint32 now = SDL_GetTicks();
    if (m_lastFrameTime == 0) {
        m_lastFrameTime = now;
    }
    Uint32 deltaTime = now - m_lastFrameTime;
    m_lastFrameTime = now;

    std::shared_ptr<Persona> persona = DataManager::getInstance().currentPersona();
    int cameraX = SCREEN_WIDTH / 2;
    int cameraY = SCREEN_HEIGHT / 2;
    if (persona != nullptr) {
        persona->tick(deltaTime);
        cameraX = persona->x();
        cameraY = persona->y();
    }

    renderBackground(cameraX, cameraY);
    renderRemotePersons(cameraX, cameraY, deltaTime);
    renderCurrentPerson();
}

void GameScene::renderCurrentPerson()
{
    std::shared_ptr<Persona> persona = DataManager::getInstance().currentPersona();

    if(nullptr == persona || nullptr == m_pRenderer)
    {
        return;
    }

    persona->rendererCurPersonaScaled(m_pRenderer, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 1.5f);
}

void GameScene::renderBackground(int cameraX, int cameraY)
{
    if (m_pRenderer == nullptr)
    {
        return;
    }

    if (m_pBackgroundTexture == nullptr)
    {
        SDL_SetRenderDrawColor(m_pRenderer, 18, 24, 26, 255);
        SDL_RenderClear(m_pRenderer);
        return;
    }

    const int maxX = std::max(0, m_iBackgroundWidth - SCREEN_WIDTH);
    const int maxY = std::max(0, m_iBackgroundHeight - SCREEN_HEIGHT);
    const int sourceX = std::clamp(cameraX - SCREEN_WIDTH / 2, 0, maxX);
    const int sourceY = std::clamp(cameraY - SCREEN_HEIGHT / 2, 0, maxY);

    SDL_Rect srcRect{sourceX, sourceY,
                     std::min(SCREEN_WIDTH, m_iBackgroundWidth),
                     std::min(SCREEN_HEIGHT, m_iBackgroundHeight)};
    SDL_Rect dstRect{0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

    SDL_RenderCopy(m_pRenderer, m_pBackgroundTexture, &srcRect, &dstRect);
}

void GameScene::renderRemotePersons(int cameraX, int cameraY, Uint32 deltaTime)
{
    std::vector<std::shared_ptr<Persona>> remotePersonas;
    DataManager::getInstance().getRemotePersonas(remotePersonas);
    for (const std::shared_ptr<Persona>& remotePersona : remotePersonas)
    {
        if (remotePersona == nullptr) {
            continue;
        }

        remotePersona->tick(deltaTime);
        const int screenX = SCREEN_WIDTH / 2 + (remotePersona->x() - cameraX);
        const int screenY = SCREEN_HEIGHT / 2 + (remotePersona->y() - cameraY);
        remotePersona->rendererCurPersonaScaled(m_pRenderer, screenX, screenY, 1.5f);
    }
}

void GameScene::handleEvent(const SDL_Event &e)
{
    std::shared_ptr<Persona> persona = DataManager::getInstance().currentPersona();
    if(nullptr != persona)
    {
        Uint32 deltaTime = 0;
        if (persona->handleEvent(e, m_lastFrameTime, deltaTime, true)) {
            NetClient::getInstance().syncCurrentPlayer();
        }
    }
}


void GameScene::initScene()
{
    if(nullptr == m_pRenderer)
        m_pRenderer = InterfaceManager::getInstance().renderer();

    if (m_pBackgroundTexture == nullptr && m_pRenderer != nullptr)
    {
        m_pBackgroundTexture = IMG_LoadTexture(m_pRenderer, "../images/tmwa/graphics/images/login_wallpaper.png");
        if (m_pBackgroundTexture == nullptr)
        {
            spdlog::error("加载游戏背景失败: {}", IMG_GetError());
        }
        else
        {
            SDL_QueryTexture(m_pBackgroundTexture, nullptr, nullptr, &m_iBackgroundWidth, &m_iBackgroundHeight);
        }
    }

    if (m_lastFrameTime == 0) {
        m_lastFrameTime = SDL_GetTicks();
    }
}
