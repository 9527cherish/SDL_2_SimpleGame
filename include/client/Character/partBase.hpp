#pragma once 

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "publicStuct.hpp"

class PartBase
{
public:
    PartBase();
    PartBase(const ImageSet& image, const SpriteData& sprite);
    ~PartBase();

    void update(const std::string& actionName, const Direction& direction, int deltaTime);
    void reset(const std::string& actionName, const Direction& direction);
    void render(SDL_Renderer* renderer,const std::string& actionName, 
                            Direction& dir, int& x, int& y);

    
    void setNeutralImage(const ImageSet& neutralImage);
    void setNeutralSprite(const SpriteData& neutralSprite);
    void setTexture(SDL_Texture* pTexture);

    void printTextureInfo(SDL_Texture* texture);

    void initTexture(SDL_Renderer* renderer);

private:
    ImageSet m_neutralImage;
    SpriteData m_neutralSprite;
    SDL_Texture* m_pTexture;

    int m_iFrameIndex;

    Frame m_Frame;
    int m_iDeltaTime;
};