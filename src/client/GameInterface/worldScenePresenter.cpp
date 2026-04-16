#include "worldScenePresenter.hpp"

#include <SDL2/SDL_image.h>
#include <spdlog/spdlog.h>
#include "interfaceManager.hpp"
#include "Network/netClient.hpp"
#include "persona.hpp"
#include "playerSession.hpp"
#include "remotepersona.hpp"
#include "remotePlayerStore.hpp"
#include "treeManager.hpp"

WorldScenePresenter::WorldScenePresenter()
{
}

WorldScenePresenter::~WorldScenePresenter()
{
    if (m_backgroundTexture != nullptr)
    {
        SDL_DestroyTexture(m_backgroundTexture);
        m_backgroundTexture = nullptr;
    }
}

void WorldScenePresenter::init(SDL_Renderer* renderer)
{
    if (renderer == nullptr) {
        return;
    }

    m_renderer = renderer;
    if (m_backgroundTexture == nullptr)
    {
        m_backgroundTexture = IMG_LoadTexture(m_renderer, "../images/tmwa/graphics/images/login_wallpaper.png");
        if (m_backgroundTexture == nullptr)
        {
            spdlog::error("加载游戏背景失败: {}", IMG_GetError());
        }
        else
        {
            SDL_QueryTexture(m_backgroundTexture, nullptr, nullptr, &m_backgroundWidth, &m_backgroundHeight);
        }
    }

    if (m_lastFrameTime == 0) {
        m_lastFrameTime = SDL_GetTicks();
    }
}

void WorldScenePresenter::render()
{
    Uint32 now = SDL_GetTicks();
    if (m_lastFrameTime == 0) {
        m_lastFrameTime = now;
    }
    const Uint32 deltaTime = now - m_lastFrameTime;
    m_lastFrameTime = now;

    std::shared_ptr<Persona> persona = PlayerSession::getInstance().currentPersona();
    if (persona != nullptr)
    {
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
    TreeManager::getInstance().renderTrees(m_renderer, m_camera);
    renderRemotePersons(deltaTime);
    renderCurrentPerson();
}

void WorldScenePresenter::handleEvent(const SDL_Event& e)
{
    std::shared_ptr<Persona> persona = PlayerSession::getInstance().currentPersona();
    if (persona == nullptr) {
        return;
    }

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

void WorldScenePresenter::renderCurrentPerson()
{
    std::shared_ptr<Persona> persona = PlayerSession::getInstance().currentPersona();

    if (persona == nullptr || m_renderer == nullptr)
    {
        return;
    }

    persona->rendererCurPersonaFootScaled(m_renderer, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 1.5f);
}

void WorldScenePresenter::renderBackground()
{
    if (m_renderer == nullptr)
    {
        return;
    }

    if (m_backgroundTexture == nullptr)
    {
        SDL_SetRenderDrawColor(m_renderer, 18, 24, 26, 255);
        SDL_RenderClear(m_renderer);
        return;
    }

    SDL_Rect srcRect = m_camera.backgroundSourceRect(m_backgroundWidth, m_backgroundHeight);
    SDL_Rect dstRect{0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

    SDL_RenderCopy(m_renderer, m_backgroundTexture, &srcRect, &dstRect);
}

void WorldScenePresenter::renderRemotePersons(Uint32 deltaTime)
{
    RemotePlayerStore::getInstance().advance(deltaTime);

    std::vector<std::shared_ptr<RemotePersona>> remotePersonas;
    RemotePlayerStore::getInstance().getPlayers(remotePersonas);
    for (const std::shared_ptr<RemotePersona>& remotePersona : remotePersonas)
    {
        if (remotePersona == nullptr) {
            continue;
        }

        // 远端角色按“世界坐标 - 相机坐标”换算到屏幕坐标，并统一按脚点锚定，避免动画帧切换时整体打滑。
        const SDL_Point screenPoint = m_camera.worldToScreen(remotePersona->x(), remotePersona->y());
        remotePersona->rendererCurPersonaFootScaled(m_renderer, screenPoint.x, screenPoint.y, 1.5f);
    }
}
