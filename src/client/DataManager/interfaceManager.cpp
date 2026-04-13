#include "interfaceManager.hpp"
#include "clientComonFunc.hpp"



InterfaceManager::InterfaceManager()
    : m_pWindow(nullptr)
    , m_pRenderer(nullptr)
    , m_pMainScene(std::make_unique<MainScene> ())
    , m_pSettingScene(std::make_unique<SettingScene>())
    , m_pGameScene(std::make_unique<GameScene>())
    , m_pCursor(std::make_unique<Cursor>())
{
    m_currentScene = Scene::MAIN_MENU;
    initWindow();
    m_pCursor->initCursor();
}

InterfaceManager::~InterfaceManager()
{
    m_pCursor->freeCursor();
    closeWindow();
}

InterfaceManager &InterfaceManager::getInstance()
{
    static InterfaceManager instance;
    return instance;
}

bool InterfaceManager::initWindow()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        spdlog::error("SDL初始化失败: " + std::string(SDL_GetError()));
        return false;
    }
    
    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        spdlog::error("SDL_image初始化失败: " + std::string(IMG_GetError()));
        SDL_Quit();
        return false;
    }

    m_pWindow = SDL_CreateWindow(GAME_NAME,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if (!m_pWindow) {
        spdlog::error( "无法创建窗口: " + std::string(SDL_GetError()) );
        IMG_Quit();
        SDL_Quit();
        return false;
    }
    m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_ACCELERATED);

    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
        spdlog::error("SDL 初始化失败：%s\n", SDL_GetError());
        return false;
    }

    if (!m_pRenderer) {
        spdlog::error( "无法创建渲染器: " + std::string(SDL_GetError()) );
        SDL_DestroyWindow(m_pWindow);
        IMG_Quit();
        SDL_Quit();
        return false;
    }

    return true;
}

void InterfaceManager::closeWindow()
{
        // 清理资源
    SDL_DestroyRenderer(m_pRenderer);
    SDL_DestroyWindow(m_pWindow);
    IMG_Quit();
    SDL_Quit();
}

void InterfaceManager::start()
{
    bool quit = false;
    SDL_Event e;
    
    while (!quit) {
        // 处理事件
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            handleEvent(e);
        }

        renderCurrentScene();
        SDL_Delay(30); // 约30FPS
        SDL_RenderPresent(m_pRenderer);
    }
}

void InterfaceManager::handleEvent(const SDL_Event &e)
{
    m_pCursor->handleEvent(e);
    switch (m_currentScene)
    {
    case Scene::MAIN_MENU:
        m_pMainScene->init();
        m_pMainScene->handleEvent(e);
        break;
        
    case Scene::GAME_SCENE:
        m_pGameScene->handleEvent(e);
        break;

    case Scene::SETTINGS:
        m_pSettingScene->handleEvent(e);
        break;
    
    default:
        break;
    }
}

void InterfaceManager::renderCurrentScene()
{
    switch (m_currentScene)
    {
    case Scene::MAIN_MENU:
        m_pMainScene->init();
        m_pMainScene->renderScene();
        break;

    case Scene::GAME_SCENE:
        m_pGameScene->init();
        m_pGameScene->renderScene();
        break;

    case Scene::SETTINGS:
        m_pSettingScene->init();
        m_pSettingScene->renderScene();
        break;

    default:
        break;
    }
}

Scene InterfaceManager::currentScene()
{
    return m_currentScene;
}

void InterfaceManager::setCurrentScene(const Scene &scene)
{
    m_currentScene = scene;
}

SDL_Renderer *InterfaceManager::renderer() const
{
    return m_pRenderer;
}
