#include "interfaceManager.hpp"
#include "clientComonFunc.hpp"



InterfaceManager::InterfaceManager()
    : m_pMainScene(std::make_unique<MainScene> ())
    , m_pSettingScene(std::make_unique<SettingScene>())
    , m_pGameScene(std::make_unique<GameScene>())
    , m_pCursor(std::make_unique<Cursor>())
{
    spdlog::info("InterfaceManager 初始化，当前场景: MAIN_MENU");
    initWindow();
    m_pCursor->initCursor();
}

InterfaceManager::~InterfaceManager()
{
    closeWindow();
}

InterfaceManager &InterfaceManager::getInstance()
{
    static InterfaceManager instance;
    return instance;
}

bool InterfaceManager::initWindow()
{
    spdlog::info("初始化窗口与渲染器");
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS) != 0) {
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

    if (!m_pRenderer) {
        spdlog::error( "无法创建渲染器: " + std::string(SDL_GetError()) );
        SDL_DestroyWindow(m_pWindow);
        m_pWindow = nullptr;
        IMG_Quit();
        SDL_Quit();
        return false;
    }

    spdlog::info("窗口与渲染器初始化成功");
    return true;
}

void InterfaceManager::closeWindow()
{
    if (m_pRenderer != nullptr) {
        SDL_DestroyRenderer(m_pRenderer);
        m_pRenderer = nullptr;
    }

    if (m_pWindow != nullptr) {
        SDL_DestroyWindow(m_pWindow);
        m_pWindow = nullptr;
    }

    IMG_Quit();
    SDL_Quit();
}

void InterfaceManager::start()
{
    spdlog::info("进入主循环");
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
    spdlog::info("切换场景到: {}", static_cast<int>(scene));
    m_currentScene = scene;
}

SDL_Renderer *InterfaceManager::renderer() const
{
    return m_pRenderer;
}
