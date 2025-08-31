#include "partBase.hpp"
#include "loadXml.hpp"

PartBase::PartBase()
{
}

PartBase::PartBase(const ImageSet &image, const SpriteData &sprite)
    :m_imageSet(image)
    ,m_spriteData(sprite)
{
}

PartBase::~PartBase()
{
    if(m_pTexture)
        SDL_DestroyTexture(m_pTexture);
}

void PartBase::update(const CharaAction &actionName, const CharaDirection &direction, int deltaTime)
{

    AnimationSequence animationSequence = m_spriteData.animations[Action(actionName, direction)];
    if(animationSequence.frames.size() < 1)
        return;
    else if(1 == animationSequence.frames.size())
    {
        m_Frame = animationSequence.frames[0];
        return;
    }
       
    m_Frame = animationSequence.frames[m_iFrameIndex];
    m_iDeltaTime = m_iDeltaTime + deltaTime;

    if(m_iDeltaTime > m_Frame.delay * 2/3)
    {
        if(m_iFrameIndex < (int)animationSequence.frames.size())
            m_iFrameIndex++;
        else
        {
            if(!animationSequence.loop)
            {
                m_iFrameIndex = 100;
                m_Frame = animationSequence.frames[0];
            }
            else
            {
                m_iFrameIndex = 0;
                m_Frame = animationSequence.frames[m_iFrameIndex];
            }
        }
        m_iDeltaTime = 0;
    }

    spdlog::info("file" + m_imageSet.file
                + "  action:" +  ActionMapper::to_string(actionName) 
                + "  direction:" + DirectionMapper::to_string(direction)
                + "  deltaTime:"  + std::to_string(deltaTime)
                + "  delay:" + std::to_string(m_Frame.delay)
                + "  index:" + std::to_string(m_Frame.index));
}

void PartBase::reset(const CharaAction &actionName, const CharaDirection &direction)
{
    AnimationSequence animationSequence = m_spriteData.animations[Action(actionName, direction)];
    if(animationSequence.frames.size() > 0)
    {
        m_iFrameIndex = 0;
        m_Frame = animationSequence.frames[0];
    }
}

void PartBase::render(SDL_Renderer *renderer, const std::string &actionName, CharaDirection &dir, int &x, int &y)
{
    if(nullptr == m_pTexture)
    return;

    int cols, rows;
    cols = rows = 0;
    SDL_QueryTexture(m_pTexture, NULL, NULL, &cols, &rows);
    cols /= m_imageSet.frameWidth;
    rows /= m_imageSet.frameHeight;
    
    // int frame_x = (m_Frame.index % cols) * m_neutralImage.frameWidth;
    // int frame_y = (m_Frame.index % rows) * m_neutralImage.frameHeight;
    int frame_x = (m_Frame.index % cols) * m_imageSet.frameWidth;
    int frame_y = (m_Frame.index / cols) * m_imageSet.frameWidth;

    // 源矩形和目标矩形
    SDL_Rect src_rect = {frame_x, frame_y, m_imageSet.frameWidth, m_imageSet.frameHeight};
    SDL_Rect dest_rect = {x + (64-m_imageSet.frameWidth)/2 + m_Frame.offsetX 
                        , y + (64-m_imageSet.frameWidth)/2 + m_Frame.offsetY/2 
                        , m_imageSet.frameWidth, m_imageSet.frameHeight};
    // 渲染
    SDL_RenderCopy(renderer, m_pTexture, &src_rect, &dest_rect);
    // spdlog::info("file" + m_neutralImage.file
    //             + "  x:" + std::to_string(x)
    //             + "  y:" + std::to_string(y));
}

void PartBase::setTexture(SDL_Texture *pTexture)
{
    m_pTexture = pTexture;
}

// void PartBase::printTextureInfo(SDL_Texture *texture)
// {
    
// }

void PartBase::initTexture(SDL_Renderer *renderer)
{
    std::string pngPath;
    if(!m_imageSet.imageSetPath.empty())
        pngPath = LoadXml::m_xmlPath + m_imageSet.imageSetPath;
    else if(!m_spriteData.imageSetSrc.empty())
    {
        std::istringstream srcPath(m_spriteData.imageSetSrc);
        std::string tmpPath = "";
        // 用 | 分割路径和参数
        if (std::getline(srcPath, tmpPath, '|')) {
            pngPath = LoadXml::m_xmlPath + tmpPath;
        } 
    }
    else{
        m_pTexture = nullptr;
        return;
    }


    SDL_Surface* surface = IMG_Load(pngPath.c_str());
    
    if (!surface) {
        spdlog::info("Failed to load image: " + std::string(IMG_GetError()));
        return;
    }

    m_pTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

void PartBase::setImageSet(const ImageSet &imageSet)
{
    m_imageSet = imageSet;
}

ImageSet PartBase::imageSet()
{
    return m_imageSet;
}

void PartBase::setSpriteData(const SpriteData &spriteData)
{
    m_spriteData = spriteData;
}

SpriteData PartBase::spriteData()
{
    return m_spriteData;
}