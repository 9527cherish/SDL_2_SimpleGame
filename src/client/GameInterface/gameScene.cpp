#include "gameScene.hpp"
#include "interfaceManager.hpp"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

void GameScene::renderScene()
{
    m_worldPresenter.render();
}

void GameScene::handleEvent(const SDL_Event &e)
{
    m_worldPresenter.handleEvent(e);
}


void GameScene::initScene()
{
    if(nullptr == m_pRenderer)
        m_pRenderer = InterfaceManager::getInstance().renderer();
    m_worldPresenter.init(m_pRenderer);
}
