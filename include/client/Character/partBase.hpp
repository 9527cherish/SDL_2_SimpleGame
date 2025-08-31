#pragma once 

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "characterStruct.hpp"

class PartBase
{
public:
    PartBase();
    PartBase(const ImageSet& image, const SpriteData& sprite);
    ~PartBase();

    // 更新数据
    void update(const CharaAction& actionName, const CharaDirection& direction, int deltaTime);
    // 重置数据
    void reset(const CharaAction& actionName, const CharaDirection& direction);
    // 渲染数据
    void render(SDL_Renderer* renderer,const std::string& actionName, 
                            CharaDirection& dir, int& x, int& y);

    void setTexture(SDL_Texture* pTexture);
    // 打印Texture信息
    // void printTextureInfo(SDL_Texture* texture);
    // 初始化Texture
    void initTexture(SDL_Renderer* renderer);

    void setImageSet(const ImageSet& imageSet);
    ImageSet imageSet();

    void setSpriteData(const SpriteData& spriteData);
    SpriteData spriteData();

private:
    ImageSet m_imageSet;
    SpriteData m_spriteData;
    SDL_Texture* m_pTexture;

    int m_iFrameIndex;
    Frame m_Frame;
    int m_iDeltaTime;

    std::string m_pngPath;   // 所在的图片路径
    std::map<std::string, std::vector<std::string> > colorGroups; // 颜色组列表
};