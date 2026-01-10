#pragma once 


#include "gameInterface.hpp"
#include "button.hpp"
#include <memory>
#include <vector>

class Persona;

class MainScene : public GameInterface
{
public:
    MainScene();
    
    // 处理事件
    void handleEvent(const SDL_Event &e) override;
    void handleStartEvent(const SDL_Event &e);
    void handleSettingEvent(const SDL_Event &e); 


    // 初始化所有控件
    void initButton();
    void initScene() override;
    void initPersonas();
    
    // 渲染控件
    void renderScene() override;
    void renderButton();
    void renderPersonas();
    void renderCurrentPerson();

    
private:
    // 主菜单按钮
    std::unique_ptr<Button> m_pStartButton;
    std::unique_ptr<Button> m_pSettingsButton;
    std::unique_ptr<Button> m_pExitButton;
    // 翻页按钮
    std::unique_ptr<Button> m_pLeftArrow;
    std::unique_ptr<Button> m_pRightArrow;

    // 设置菜单元素
    std::unique_ptr<Button> m_pBackButton;
    std::unique_ptr<Button> m_pSaveButton;
    
    TTF_Font* m_pTitleFont;
    TTF_Font* m_pButtonFont;
    TTF_Font* m_pLabelFont;

    SDL_Renderer* m_pRenderer;

    // 人物形象展示
    std::map<uint, std::unique_ptr<Button>> m_personasButtonMap;

    // 最大几页
    int m_maxPage;
    // 当前第几页
    int m_page;
    // 第几个人物
    int m_number;


};

