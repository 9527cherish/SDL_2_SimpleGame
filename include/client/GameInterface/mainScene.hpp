#pragma once 


#include "gameInterface.hpp"
#include "button.hpp"
#include <memory>
#include <vector>

class Persona;

// MainScene 负责角色选择、主菜单按钮和进入游戏流程
class MainScene : public GameInterface
{
public:
    MainScene();
    ~MainScene() override;
    
    // 处理主菜单中的输入事件。
    void handleEvent(const SDL_Event &e) override;
    // 处理开始游戏按钮点击逻辑。
    void handleStartEvent(const SDL_Event &e);
    // 处理进入设置场景的逻辑。
    void handleSettingEvent(const SDL_Event &e); 


    // 初始化主菜单按钮控件。
    void initButton();
    // 初始化主菜单场景。
    void initScene() override;
    // 初始化角色分页信息。
    void initPersonas();
    
    // 渲染主菜单整体内容。
    void renderScene() override;
    // 渲染主菜单按钮区域。
    void renderButton();
    // 渲染可选角色列表。
    void renderPersonas();
    // 渲染当前选中的角色预览。
    void renderCurrentPerson();

    
private:
    // 主菜单按钮
    // 开始游戏按钮
    std::unique_ptr<Button> m_pStartButton;
    // 设置按钮
    std::unique_ptr<Button> m_pSettingsButton;
    // 退出按钮
    std::unique_ptr<Button> m_pExitButton;
    // 角色列表向左翻页按钮
    std::unique_ptr<Button> m_pLeftArrow;
    // 角色列表向右翻页按钮
    std::unique_ptr<Button> m_pRightArrow;

    // 预留的返回按钮
    std::unique_ptr<Button> m_pBackButton;
    // 预留的保存设置按钮
    std::unique_ptr<Button> m_pSaveButton;
    
    // 主标题字体
    TTF_Font* m_pTitleFont = nullptr;
    // 按钮文字字体
    TTF_Font* m_pButtonFont = nullptr;
    // 标签文字字体
    TTF_Font* m_pLabelFont = nullptr;

    // 场景渲染器
    SDL_Renderer* m_pRenderer = nullptr;

    // 角色选择按钮表
    std::map<unsigned int, std::unique_ptr<Button>> m_personasButtonMap;

    // 当前角色列表总页数
    int m_maxPage = 1;
    // 当前浏览到的页码
    int m_page = 0;
    // 当前页中被选中的角色序号
    int m_number = -1;


};
