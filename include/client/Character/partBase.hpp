#pragma once 

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "characterStruct.hpp"

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

    // 更新数据
    void update(const CharaAction& actionName, const CharaDirection& direction, int deltaTime);
    // 重置数据
    void reset(const CharaAction& actionName, const CharaDirection& direction);
    // 渲染数据
    void render(SDL_Renderer* renderer,const CharaAction& actionName, 
                            CharaDirection& dir, int& x, int& y);
    SDL_Rect renderRect(const CharaAction& actionName, const CharaDirection& dir, int x, int y) const;
    // 处理事件
    bool handleEvent(const SDL_Event& e, const CharaAction& actionName, const CharaDirection& direction);

    void setTexture(SDL_Texture* pTexture);

    // 初始化Texture
    void initTexture(SDL_Renderer* renderer);

    void setImageSet(const ImageSet& imageSet);
    ImageSet imageSet();
    const ImageSet& imageSet() const;

    void setSpriteData(const SpriteData& spriteData);
    SpriteData spriteData();
    const SpriteData& spriteData() const;

    void setVariant(int variant);
    int variant() const;

private:
    ImageSet m_imageSet;
    SpriteData m_spriteData;
    SDL_Texture* m_pTexture;

    bool m_initTexture;

    int m_iFrameIndex;
    Frame m_Frame;
    int m_iDeltaTime;
    int m_variant = -1;

    std::string m_pngPath;   // 所在的图片路径
    std::map<std::string, std::vector<std::string> > colorGroups; // 颜色组列表
};
