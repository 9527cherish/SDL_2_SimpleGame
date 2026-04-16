#pragma once 

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "characterStruct.hpp"
#include "messageInfo.hpp"

// PartBase 表示人物的一个可独立动画更新与渲染的部件。
class PartBase
{
public:
    PartBase();
    PartBase(const ImageSet& image, const SpriteData& sprite);
    // PartBase 会被 Persona 深拷贝；这里需要显式定义资源语义，避免 SDL_Texture 被浅拷贝后重复释放。
    PartBase(const PartBase& other);
    PartBase& operator=(const PartBase& other);
    PartBase(PartBase&& other) noexcept;
    PartBase& operator=(PartBase&& other) noexcept;
    ~PartBase();

    // 按动作、方向和时间推进当前部件动画。
    void update(const CharaAction& actionName, const CharaDirection& direction, int deltaTime);
    // 把当前部件动画重置到起始帧。
    void reset(const CharaAction& actionName, const CharaDirection& direction);
    // 以原始尺寸渲染当前部件。
    void render(SDL_Renderer* renderer,const CharaAction& actionName, 
                            CharaDirection& dir, int& x, int& y);
    // 按指定缩放比例渲染当前部件。
    void renderScaled(SDL_Renderer* renderer, const CharaAction& actionName,
                            CharaDirection& dir, int& x, int& y, float scale);
    // 计算当前部件应渲染到的目标矩形。
    SDL_Rect renderRect(const CharaAction& actionName, const CharaDirection& dir, int x, int y) const;
    // 处理部件级输入事件。
    bool handleEvent(const SDL_Event& e, const CharaAction& actionName, const CharaDirection& direction);

    // 直接设置已经创建好的纹理资源。
    void setTexture(SDL_Texture* pTexture);

    // 延迟初始化当前部件所需的纹理。
    void initTexture(SDL_Renderer* renderer);

    // 设置当前部件绑定的图片集数据。
    void setImageSet(const ImageSet& imageSet);
    // 返回图片集数据的拷贝。
    ImageSet imageSet();
    // 返回图片集数据的常量引用。
    const ImageSet& imageSet() const;

    // 设置当前部件绑定的精灵动作数据。
    void setSpriteData(const SpriteData& spriteData);
    // 返回精灵动作数据的拷贝。
    SpriteData spriteData();
    // 返回精灵动作数据的常量引用。
    const SpriteData& spriteData() const;

    // 设置当前部件使用的变体编号。
    void setVariant(int variant);
    // 返回当前部件使用的变体编号。
    int variant() const;
    // 导出当前部件用于远端同步的帧信息。
    PartSyncInfo syncInfo() const;
    // 应用远端同步过来的帧信息。
    void applySyncInfo(const PartSyncInfo& syncInfo);

private:
    ImageSet m_imageSet;                  // 当前部件引用的图片集配置。
    SpriteData m_spriteData;              // 当前部件的动作与帧数据。
    SDL_Texture* m_pTexture = nullptr;    // 当前部件渲染所使用的纹理。

    bool m_initTexture = false;           // 当前纹理是否已经初始化完成。

    int m_iFrameIndex = 0;                // 当前动画播放到的帧下标。
    Frame m_Frame;                        // 当前正在使用的帧数据。
    int m_iDeltaTime = 0;                 // 当前帧已经累计的时间。
    int m_variant = -1;                   // 当前部件使用的图片变体编号。

    std::string m_pngPath;   // 当前部件纹理所在的图片路径。
    std::map<std::string, std::vector<std::string> > colorGroups; // 当前部件可用的颜色组列表。
};
