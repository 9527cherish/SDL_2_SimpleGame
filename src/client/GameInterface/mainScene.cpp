#include "mainScene.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "sceneStruct.hpp"
#include "interfaceManager.hpp"
#include "dataManager.hpp"


MainScene::MainScene()
    : m_page(0)
    , m_number(-1)
{
    if (TTF_Init() == -1) {
        spdlog::error("TTF_Init 初始化失败:" + std::string(TTF_GetError()));
    }

    m_pTitleFont = TTF_OpenFont("SourceHanSansCN-Regular.otf", 16);
    if (!m_pTitleFont) {
        spdlog::error("MainScene Title加载字体失败:"  + std::string(TTF_GetError()));
    }

    m_pButtonFont = TTF_OpenFont("SourceHanSansCN-Regular.otf", 16);
    if (!m_pButtonFont) {
        spdlog::error("MainScene Button加载字体失败:"  + std::string(TTF_GetError()));
    }

    m_pLabelFont = TTF_OpenFont("SourceHanSansCN-Regular.otf", 16);
    if (!m_pLabelFont) {
        spdlog::error("MainScene Label加载字体失败:"  + std::string(TTF_GetError()));
    }
}

void MainScene::renderScene()
{
    if(nullptr == m_pRenderer)
        return;

    SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);
    SDL_RenderClear(m_pRenderer);
    renderButton();
    renderPersonas();

    if(-1 != m_number)
    {
        renderCurrentPerson();
    }
}

void MainScene::handleEvent(const SDL_Event &e)
{
    m_pStartButton->handleEvent(e);  
    m_pSettingsButton->handleEvent(e);
    m_pExitButton->handleEvent(e);
    m_pLeftArrow->handleEvent(e);
    m_pRightArrow->handleEvent(e);

    for(const auto& [key, value] : m_personasMap)
    {
        if(value->handleEvent(e))
        {
            m_number = key;
            std::vector<std::shared_ptr<Persona>> personas;
            DataManager::getInstance().setCurrentPerson(m_page*10 + m_number);
        }   
    }
    if(-1 != m_number)
    {
        std::shared_ptr<Persona> persona = DataManager::getInstance().currentPersona();
        if(nullptr != persona)
        {
            persona->renderer(m_pRenderer, 450, 250);
        }

    }
}

void MainScene::initButton()
{
    if(nullptr == m_pRenderer)
        m_pRenderer = InterfaceManager::getInstance().renderer();
    
    int buttonWidth = 240;
    int buttonHeight = 60;
    int buttonX = 100;
    int buttonY = 250;
    int buttonSpacing = 120;
    
    m_pStartButton = std::make_unique<Button>(
        m_pRenderer, m_pButtonFont, "开始游戏",
        buttonX, buttonY,
        buttonWidth, buttonHeight,
        SDL_Color{108, 92, 231, 200},    // 正常颜色
        SDL_Color{108, 92, 231, 255},    // 悬停颜色
        SDL_Color{85, 72, 189, 255},     // 按下颜色
        SDL_Color{255, 255, 255, 255}         // 文本颜色
    );

    buttonY += buttonSpacing;
    m_pSettingsButton = std::make_unique<Button>(
        m_pRenderer, m_pButtonFont, "游戏设置",
        buttonX, buttonY,
        buttonWidth, buttonHeight,
        SDL_Color{45, 52, 54, 200},      // 正常颜色
        SDL_Color{45, 52, 54, 255},      // 悬停颜色
        SDL_Color{30, 35, 36, 255},      // 按下颜色
        SDL_Color{108, 92, 231, 255}          // 文本颜色
    );

    buttonY += buttonSpacing;
    m_pExitButton = std::make_unique<Button>(
        m_pRenderer, m_pButtonFont, "退出游戏",
        buttonX, buttonY,
        buttonWidth, buttonHeight,
        SDL_Color{45, 52, 54, 200},      // 正常颜色
        SDL_Color{45, 52, 54, 255},      // 悬停颜色
        SDL_Color{30, 35, 36, 255},      // 按下颜色
        SDL_Color{253, 121, 168, 255}         // 文本颜色
    );

    // 初始化设置菜单元素
    m_pBackButton = std::make_unique<Button>(
        m_pRenderer, m_pButtonFont, "返回",
        50, 50,
        120, 50,
        SDL_Color{45, 52, 54, 200},      // 正常颜色
        SDL_Color{45, 52, 54, 255},      // 悬停颜色
        SDL_Color{30, 35, 36, 255},      // 按下颜色
        SDL_Color{255, 255, 255, 255}         // 文本颜色
    );
    m_pBackButton->setVisible(false);

    m_pSaveButton = std::make_unique<Button>(
        m_pRenderer, m_pButtonFont, "保存设置",
        SCREEN_WIDTH - 170, 50,
        120, 50,
        SDL_Color{108, 92, 231, 200},    // 正常颜色
        SDL_Color{108, 92, 231, 255},    // 悬停颜色
        SDL_Color{85, 72, 189, 255},     // 按下颜色
        SDL_Color{255, 255, 255, 255}         // 文本颜色
    );
    m_pSaveButton->setVisible(false);


    m_pLeftArrow = std::make_unique<Button>(
        m_pRenderer, m_pButtonFont, "<",
        int(SCREEN_WIDTH/3*2-100), SCREEN_HEIGHT - 100,
        60, 60,
        SDL_Color{45, 52, 54, 200},      // 正常颜色
        SDL_Color{45, 52, 54, 255},      // 悬停颜色
        SDL_Color{30, 35, 36, 255},      // 按下颜色
        SDL_Color{108, 92, 231, 255}          // 文本颜色
    );
    m_pLeftArrow->setBorder(false);

    m_pRightArrow = std::make_unique<Button>(
        m_pRenderer, m_pButtonFont, ">",
        int(SCREEN_WIDTH/3*2 + 100), SCREEN_HEIGHT - 100,
        60, 60,
        SDL_Color{45, 52, 54, 200},      // 正常颜色
        SDL_Color{45, 52, 54, 255},      // 悬停颜色
        SDL_Color{30, 35, 36, 255},      // 按下颜色
        SDL_Color{108, 92, 231, 255}          // 文本颜色
    );
    m_pRightArrow->setBorder(false);

    // 初始化人物界面
    {
        int x, y;
        for(int i = 0; i < 5; i++)
        {
            for(int j = 0; j < 2; j++)
            {
                x = 100*i + 432;
                y = 100*j + 332;

                m_personasMap[j*5+i] = std::make_unique<Button>(
                    m_pRenderer, m_pButtonFont, "",
                    x, y, 100, 100, 
                    SDL_Color{45, 52, 54, 200},      // 正常颜色
                    SDL_Color{45, 52, 54, 255},      // 悬停颜色
                    SDL_Color{30, 35, 36, 255},      // 按下颜色
                    SDL_Color{108, 92, 231, 255}          // 文本颜色
                );
                // m_personasMap[j*5+i]->setBorder(false);
            }
        }
    }
}

void MainScene::renderButton()
{
    // 渲染主菜单按钮
    m_pStartButton->renderButton();
    m_pSettingsButton->renderButton();
    m_pExitButton->renderButton();
    m_pLeftArrow->renderButton();
    m_pRightArrow->renderButton();

    for(const auto& [key, value] : m_personasMap)
    {
        value->renderButton();
    }
}

void MainScene::renderPersonas()
{
    std::vector<std::shared_ptr<Persona>> personas;
    DataManager::getInstance().getData(personas);

    int x, y;
    for(int i = 0; i < 5; i++)
    {
        for(int j = 0; j < 2; j++)
        {
            x = 100*i + 450;
            y = 100*j + 350;
            std::shared_ptr<Persona> persona = personas[j*5+i];
            persona->renderer(m_pRenderer, x, y);
        }
    }
}

void MainScene::renderCurrentPerson()
{
    std::shared_ptr<Persona> persona = DataManager::getInstance().currentPersona();

    if(nullptr != persona)
    {
        int x = 600;
        int y = 200;
        persona->renderer(m_pRenderer, x, y);
    }
}

void MainScene::initScene()
{
    if(nullptr == m_pRenderer)
        m_pRenderer = InterfaceManager::getInstance().renderer();

    initButton();
}
