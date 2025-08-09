#pragma once


#include "PartBase.hpp"

class Neutral : public PartBase
{
 
public:
    Neutral();
    Neutral(const ImageSet& image, const SpriteData& sprite);
    ~Neutral();

    virtual void update(const std::string& actionName, const Direction& direction, int deltaTime) override;
    virtual void reset(const std::string& actionName, const Direction& direction) override;
    virtual void render(SDL_Renderer* renderer,const std::string& actionName, 
                            Direction& dir, int& x, int& y) override;

    
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
