#include "gameScene.hpp"
#include <SDL2/SDL_image.h>
#include <spdlog/spdlog.h>
#include "persona.hpp"
#include "remotepersona.hpp"
#include "interfaceManager.hpp"
#include "dataManager.hpp"
#include "treeManager.hpp"
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
    if (persona != nullptr) {
        // 本地角色的逐帧更新同时作为联机同步触发点：位置或动作变了才发消息。
        if (persona->tick(deltaTime)) {
            NetClient::getInstance().syncCurrentPlayer();
        }
        m_camera.follow(persona->x(), persona->y());
    }
    else
    {
        m_camera.follow(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
    }

    renderBackground();
    TreeManager::getInstance().renderTrees(m_pRenderer, m_camera);
    renderRemotePersons(deltaTime);
    renderCurrentPerson();
}

void GameScene::renderCurrentPerson()
{
    std::shared_ptr<Persona> persona = DataManager::getInstance().currentPersona();

    if(nullptr == persona || nullptr == m_pRenderer)
    {
        return;
    }

    persona->rendererCurPersonaFootScaled(m_pRenderer, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 1.5f);
}

void GameScene::renderBackground()
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

    SDL_Rect srcRect = m_camera.backgroundSourceRect(m_iBackgroundWidth, m_iBackgroundHeight);
    SDL_Rect dstRect{0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

    SDL_RenderCopy(m_pRenderer, m_pBackgroundTexture, &srcRect, &dstRect);
}

void GameScene::renderRemotePersons(Uint32 deltaTime)
{
    DataManager::getInstance().advanceRemotePersonas(deltaTime);

    std::vector<std::shared_ptr<RemotePersona>> remotePersonas;
    DataManager::getInstance().getRemotePersonas(remotePersonas);
    for (const std::shared_ptr<RemotePersona>& remotePersona : remotePersonas)
    {
        if (remotePersona == nullptr) {
            continue;
        }

        // 远端角色按“世界坐标 - 相机坐标”换算到屏幕坐标，并统一按脚点锚定，避免动画帧切换时整体打滑。
        const SDL_Point screenPoint = m_camera.worldToScreen(remotePersona->x(), remotePersona->y());
        remotePersona->rendererCurPersonaFootScaled(m_pRenderer, screenPoint.x, screenPoint.y, 1.5f);
    }
}

void GameScene::handleEvent(const SDL_Event &e)
{
    std::shared_ptr<Persona> persona = DataManager::getInstance().currentPersona();
    if(nullptr != persona)
    {
        if (e.type == SDL_KEYDOWN && !e.key.repeat && e.key.keysym.sym == SDLK_j)
        {
            const int treeId = TreeManager::getInstance().findNearestAliveTree(persona->x(), persona->y(), 140);
            if (treeId > 0) {
                NetClient::getInstance().hitTree(treeId, 1);
            }
        }

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
