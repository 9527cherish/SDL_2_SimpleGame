#include "interfaceManager.hpp"
#include "clientComonFunc.hpp"



InterfaceManager::InterfaceManager()
    : m_window(nullptr)
    , m_renderer(nullptr)
    , m_mainScene(new MainScene())
    , m_settingScene(new SettingScene())
    , m_gameScene(new GameScene())
{

    m_currentScene = Scene::MAIN_MENU;
}

InterfaceManager::~InterfaceManager()
{
    if(nullptr != m_mainScene)
    {
        delete m_mainScene;
        m_mainScene = nullptr;
    }

    if(nullptr != m_settingScene)
    {
        delete m_settingScene;
        m_settingScene = nullptr;
    }

    if(nullptr != m_gameScene)
    {
        delete m_gameScene;
        m_gameScene = nullptr;
    }
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

    m_window = SDL_CreateWindow(GAME_NAME,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if (!m_window) {
        spdlog::error( "无法创建窗口: " + std::string(SDL_GetError()) );
        IMG_Quit();
        SDL_Quit();
        return false;
    }
    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);

    if (!m_renderer) {
        spdlog::error( "无法创建渲染器: " + std::string(SDL_GetError()) );
        SDL_DestroyWindow(m_window);
        IMG_Quit();
        SDL_Quit();
        return false;
    }

    return true;
}

void InterfaceManager::closeWindow()
{
        // 清理资源
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    IMG_Quit();
    SDL_Quit();
}

void InterfaceManager::start()
{
    initWindow();

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        // 处理事件
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }
        handleEvent();
        SDL_Delay(30); // 约30FPS
            // 更新屏幕
        SDL_RenderPresent(m_renderer);
    }
    closeWindow();
}

void InterfaceManager::handleEvent()
{
    switch (m_currentScene)
    {
    case Scene::MAIN_MENU:
        m_mainScene->initButton();
        m_mainScene->renderScene();
        m_mainScene->handleEvent();
        break;
        
    case Scene::GAME_SCENE:
        m_gameScene->renderScene();
        m_gameScene->handleEvent();
        break;

    case Scene::SETTINGS:
        m_settingScene->renderScene();
        m_settingScene->handleEvent();
        break;
    
    default:
        break;
    }
}

Scene InterfaceManager::currentScene()
{
    return m_currentScene;
}

SDL_Renderer *InterfaceManager::renderer() const
{
    return m_renderer;
}
