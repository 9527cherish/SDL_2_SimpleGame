#pragma once 


#include "gameInterface.hpp"
#include "button.hpp"
#include <memory>
#include <vector>

class MainScene : public GameInterface
{
public:
    MainScene();
    
    void handleEvent(const SDL_Event &e) override;

    // 初始化所有控件
    void initButton();
    void initScene() override;
    
    // 渲染控件
    void renderScene() override;
    void renderButton();
    void renderPersona();

    
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
    std::vector<std::unique_ptr<Button>> m_personasList;

    // std::shared_ptr<Persona> m_pCurrentPerson;

};

