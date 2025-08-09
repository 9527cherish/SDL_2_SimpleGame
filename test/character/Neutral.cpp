#include "Neutral.hpp"
#include "createCharacter.hpp"

Neutral::Neutral(const ImageSet &image, const SpriteData &sprite)
    : m_neutralImage(image)
    , m_neutralSprite(sprite)
{
    m_iFrameIndex = 0;
    m_iDeltaTime = 0;
}

Neutral::Neutral()
{
    m_iFrameIndex = 0;
    m_iDeltaTime = 0;
}

Neutral::~Neutral()
{
}

void Neutral::update(const std::string &actionName, const Direction &direction, int deltaTime)
{
    
    AnimationSequence animationSequence = getFrameFromSprite(m_neutralSprite, actionName, direction);
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
        if(m_iFrameIndex < animationSequence.frames.size())
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


    spdlog::info("file" + m_neutralImage.file
                + "  action:" +  actionName  
                + "  direction:" + std::to_string((int)direction)
                + "  deltaTime:"  + std::to_string(deltaTime)
                + "  delay:" + std::to_string(m_Frame.delay)
                + "  index:" + std::to_string(m_Frame.index));
    
}

void Neutral::reset(const std::string &actionName, const Direction &direction)
{
    AnimationSequence animationSequence = getFrameFromSprite(m_neutralSprite, actionName, direction);
    if(animationSequence.frames.size() > 0)
    {
        m_iFrameIndex = 0;
        m_Frame = animationSequence.frames[0];
    }
}

void Neutral::render(SDL_Renderer *renderer, const std::string &actionName, Direction &dir, int &x, int &y)
{
    if(nullptr == m_pTexture)
        return;

    int cols, rows;
    cols = rows = 0;
    SDL_QueryTexture(m_pTexture, NULL, NULL, &cols, &rows);
    cols /= m_neutralImage.frameWidth;
    rows /= m_neutralImage.frameHeight;
    
    // int frame_x = (m_Frame.index % cols) * m_neutralImage.frameWidth;
    // int frame_y = (m_Frame.index % rows) * m_neutralImage.frameHeight;
    int frame_x = (m_Frame.index % cols) * m_neutralImage.frameWidth;
    int frame_y = (m_Frame.index / cols) * m_neutralImage.frameWidth;

    // 源矩形和目标矩形
    SDL_Rect src_rect = {frame_x, frame_y, m_neutralImage.frameWidth, m_neutralImage.frameHeight};
    SDL_Rect dest_rect = {x + (64-m_neutralImage.frameWidth)/2 + m_Frame.offsetX 
                        , y + (64-m_neutralImage.frameWidth)/2 + m_Frame.offsetY/2 
                        , m_neutralImage.frameWidth, m_neutralImage.frameHeight};
    // 渲染
    SDL_RenderCopy(renderer, m_pTexture, &src_rect, &dest_rect);
    // spdlog::info("file" + m_neutralImage.file
    //             + "  x:" + std::to_string(x)
    //             + "  y:" + std::to_string(y));
}

void Neutral::setNeutralImage(const ImageSet &neutralImage)
{
    m_neutralImage = neutralImage;
}

void Neutral::setNeutralSprite(const SpriteData &neutralSprite)
{
    m_neutralSprite = neutralSprite;
}

void Neutral::setTexture(SDL_Texture *pTexture)
{
    if(nullptr != pTexture)
        m_pTexture = pTexture;
}

void Neutral::printTextureInfo(SDL_Texture *texture)
{
    if (!texture) {
        spdlog::error("Texture is null!");
        return;
    }

    // 定义变量存储属性
    Uint32 format;       // 像素格式（如 SDL_PIXELFORMAT_RGBA8888）
    int access;          // 访问模式（如 SDL_TEXTUREACCESS_STATIC）
    int width, height;   // 宽和高

    // 查询纹理属性
    if (SDL_QueryTexture(texture, &format, &access, &width, &height) != 0) {
        spdlog::error("Failed to query texture: " + std::string(SDL_GetError()) );
        return;
    }

    // 打印属性
    spdlog::info("Texture Information:");
    spdlog::info("  Width: " + std::to_string(width) +"px");
    spdlog::info("  Height: " + std::to_string(height) +"px");
    spdlog::info("  Pixel Format: ");
    
    // 转换格式枚举为可读字符串
    switch (format) {
        case SDL_PIXELFORMAT_RGBA8888: spdlog::info("RGBA8888"); break;
        case SDL_PIXELFORMAT_RGB888:  spdlog::info("RGB888"); break;
        case SDL_PIXELFORMAT_ARGB8888: spdlog::info("ARGB8888"); break;
        case SDL_PIXELFORMAT_RGBA4444: spdlog::info("RGBA4444"); break;
        case SDL_PIXELFORMAT_RGB565:   spdlog::info("RGB565"); break;
        default: spdlog::info(" (unknown)");
    }
    const char* name = SDL_GetPixelFormatName(format);
    if(nullptr != name)
    {
        spdlog::info(std::string(name));
    }

    spdlog::info( "  Access Mode: ");
    switch (access) {
        case SDL_TEXTUREACCESS_STATIC:  spdlog::info("STATIC (不可变内容)"); break;
        case SDL_TEXTUREACCESS_STREAMING: spdlog::info( "STREAMING (频繁更新)"); break;
        case SDL_TEXTUREACCESS_TARGET:  spdlog::info( "TARGET (可作为渲染目标)"); break;
        default: spdlog::info("(unknown)");
    }

}

void Neutral::initTexture(SDL_Renderer* renderer)
{
    std::string pngPath = "../../images/tmwa/" + m_neutralImage.imageSetPath;
    SDL_Surface* surface = IMG_Load(pngPath.c_str());
    
    if (!surface) {
        spdlog::info("Failed to load image: " + std::string(IMG_GetError()));
        return;
    }

    m_pTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if(nullptr != m_pTexture)
    {
        printTextureInfo(m_pTexture);
    }
}
