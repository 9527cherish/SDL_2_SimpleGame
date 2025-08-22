#include "partBase.hpp"

PartBase::PartBase()
{
}

PartBase::PartBase(const ImageSet &image, const SpriteData &sprite)
{
}

PartBase::~PartBase()
{
}

void PartBase::update(const std::string &actionName, const Direction &direction, int deltaTime)
{
}

void PartBase::reset(const std::string &actionName, const Direction &direction)
{
}

void PartBase::render(SDL_Renderer *renderer, const std::string &actionName, Direction &dir, int &x, int &y)
{
}

void PartBase::setNeutralImage(const ImageSet &neutralImage)
{
}

void PartBase::setNeutralSprite(const SpriteData &neutralSprite)
{
}

void PartBase::setTexture(SDL_Texture *pTexture)
{
}

void PartBase::printTextureInfo(SDL_Texture *texture)
{
}

void PartBase::initTexture(SDL_Renderer *renderer)
{
}
