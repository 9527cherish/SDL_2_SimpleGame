#pragma once 

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "publicStruct.hpp"

class PartBase
{
public:
    PartBase();
    PartBase(const ImageSet& image, const SpriteData& sprite);
    ~PartBase();

    void update(const std::string& actionName, const CharaDirection& direction, int deltaTime);
    void reset(const std::string& actionName, const CharaDirection& direction);
    void render(SDL_Renderer* renderer,const std::string& actionName, 
                            CharaDirection& dir, int& x, int& y);

    
    void setNeutralImage(const ImageSet& neutralImage);
    void setNeutralSprite(const SpriteData& neutralSprite);
    void setTexture(SDL_Texture* pTexture);

    void printTextureInfo(SDL_Texture* texture);

    void initTexture(SDL_Renderer* renderer);

    void setImageSet(const ImageSet& imageSet);
    void setSpriteData(const SpriteData& spriteData);

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