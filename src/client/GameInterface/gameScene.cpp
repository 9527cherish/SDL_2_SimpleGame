#include "gameScene.hpp"
#include "persona.hpp"
#include "interfaceManager.hpp"
#include "dataManager.hpp"
#include "netClient.hpp"

void GameScene::renderScene()
{
    SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);
    SDL_RenderClear(m_pRenderer);

    Uint32 now = SDL_GetTicks();
    if (m_lastFrameTime == 0) {
        m_lastFrameTime = now;
    }
    Uint32 deltaTime = now - m_lastFrameTime;
    m_lastFrameTime = now;

    std::shared_ptr<Persona> persona = DataManager::getInstance().currentPersona();
    if (persona != nullptr) {
        persona->tick(deltaTime);
    }

    std::vector<std::shared_ptr<Persona>> remotePersonas;
    DataManager::getInstance().getRemotePersonas(remotePersonas);
    for (const std::shared_ptr<Persona>& remotePersona : remotePersonas)
    {
        if (remotePersona == nullptr) {
            continue;
        }
        remotePersona->tick(deltaTime);
        remotePersona->rendererCurPersonaScaled(m_pRenderer, 1.5f);
    }

    renderCurrentPerson();
}

void GameScene::renderCurrentPerson()
{
    std::shared_ptr<Persona> persona = DataManager::getInstance().currentPersona();

    if(nullptr == persona || nullptr == m_pRenderer)
    {
        return;
    }

    persona->rendererCurPersonaScaled(m_pRenderer, 1.5f);
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
    if (m_lastFrameTime == 0) {
        m_lastFrameTime = SDL_GetTicks();
    }
}
