#include "gameScene.hpp"
#include "persona.hpp"
#include "interfaceManager.hpp"
#include "dataManager.hpp"

void GameScene::renderScene()
{
    SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);
    SDL_RenderClear(m_pRenderer);

    renderCurrentPerson();
}

void GameScene::renderCurrentPerson()
{
    std::shared_ptr<Persona> persona = DataManager::getInstance().currentPersona();

    if(nullptr == persona && nullptr == m_pRenderer)
    {
        return;
    }

    persona->rendererCurPersona(m_pRenderer);
}

void GameScene::handleEvent(const SDL_Event &e)
{
    std::shared_ptr<Persona> persona = DataManager::getInstance().currentPersona();
    if(nullptr != persona)
    {
        Uint32 lastFrameTime = SDL_GetTicks();
        Uint32 deltaTime; 
        persona->handleEvent(e, lastFrameTime, deltaTime, true);
    }
}


void GameScene::initScene()
{
    if(nullptr == m_pRenderer)
        m_pRenderer = InterfaceManager::getInstance().renderer();
}
