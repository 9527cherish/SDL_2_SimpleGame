#include "partBase.hpp"
#include "loadXml.hpp"

namespace {
const AnimationSequence* findAnimationSequence(const SpriteData& spriteData,
                                               const CharaAction& actionName,
                                               const CharaDirection& direction)
{
    auto it = spriteData.animations.find(Action(actionName, direction));
    if (it != spriteData.animations.end() && !it->second.frames.empty()) {
        return &it->second;
    }

    it = spriteData.animations.find(Action(actionName, CharaDirection::DEFAULT));
    if (it != spriteData.animations.end() && !it->second.frames.empty()) {
        return &it->second;
    }

    it = spriteData.animations.find(Action(CharaAction::STAND, direction));
    if (it != spriteData.animations.end() && !it->second.frames.empty()) {
        return &it->second;
    }

    it = spriteData.animations.find(Action(CharaAction::STAND, CharaDirection::DOWN));
    if (it != spriteData.animations.end() && !it->second.frames.empty()) {
        return &it->second;
    }

    return nullptr;
}

int resolveFrameIndex(const PartBase& part, const Frame& frame)
{
    int frameIndex = frame.index;
    const int variant = part.variant();
    const SpriteData spriteData = part.spriteData();

    if (variant >= spriteData.variantOffset) {
        frameIndex += variant - spriteData.variantOffset;
    }

    return frameIndex;
}
}

PartBase::PartBase()
    : m_initTexture(false)
    , m_iDeltaTime(0)
    , m_iFrameIndex(0)
{
}

PartBase::PartBase(const ImageSet &image, const SpriteData &sprite)
    : m_imageSet(image)
    , m_spriteData(sprite)
    , m_pTexture(nullptr)
    , m_initTexture(false)
    , m_iFrameIndex(0)
    , m_iDeltaTime(0)
{
}

PartBase::PartBase(const PartBase& other)
    : m_imageSet(other.m_imageSet)
    , m_spriteData(other.m_spriteData)
    // 纹理属于 SDL 运行时资源，拷贝对象时不共享底层指针，后续按需重新加载。
    , m_pTexture(nullptr)
    , m_initTexture(false)
    , m_iFrameIndex(other.m_iFrameIndex)
    , m_Frame(other.m_Frame)
    , m_iDeltaTime(other.m_iDeltaTime)
    , m_variant(other.m_variant)
    , m_pngPath(other.m_pngPath)
    , colorGroups(other.colorGroups)
{
}

PartBase& PartBase::operator=(const PartBase& other)
{
    if (this == &other) {
        return *this;
    }

    if (m_pTexture != nullptr) {
        SDL_DestroyTexture(m_pTexture);
    }

    m_imageSet = other.m_imageSet;
    m_spriteData = other.m_spriteData;
    // 赋值后当前对象重新回到“未加载纹理”状态，避免两个 PartBase 持有同一 SDL_Texture。
    m_pTexture = nullptr;
    m_initTexture = false;
    m_iFrameIndex = other.m_iFrameIndex;
    m_Frame = other.m_Frame;
    m_iDeltaTime = other.m_iDeltaTime;
    m_variant = other.m_variant;
    m_pngPath = other.m_pngPath;
    colorGroups = other.colorGroups;
    return *this;
}

PartBase::PartBase(PartBase&& other) noexcept
    : m_imageSet(std::move(other.m_imageSet))
    , m_spriteData(std::move(other.m_spriteData))
    , m_pTexture(other.m_pTexture)
    , m_initTexture(other.m_initTexture)
    , m_iFrameIndex(other.m_iFrameIndex)
    , m_Frame(other.m_Frame)
    , m_iDeltaTime(other.m_iDeltaTime)
    , m_variant(other.m_variant)
    , m_pngPath(std::move(other.m_pngPath))
    , colorGroups(std::move(other.colorGroups))
{
    // 移动语义转移纹理所有权，源对象必须清空指针，避免析构时二次释放。
    other.m_pTexture = nullptr;
    other.m_initTexture = false;
}

PartBase& PartBase::operator=(PartBase&& other) noexcept
{
    if (this == &other) {
        return *this;
    }

    if (m_pTexture != nullptr) {
        SDL_DestroyTexture(m_pTexture);
    }

    m_imageSet = std::move(other.m_imageSet);
    m_spriteData = std::move(other.m_spriteData);
    m_pTexture = other.m_pTexture;
    m_initTexture = other.m_initTexture;
    m_iFrameIndex = other.m_iFrameIndex;
    m_Frame = other.m_Frame;
    m_iDeltaTime = other.m_iDeltaTime;
    m_variant = other.m_variant;
    m_pngPath = std::move(other.m_pngPath);
    colorGroups = std::move(other.colorGroups);

    // 目标对象接管纹理后，源对象放弃所有权。
    other.m_pTexture = nullptr;
    other.m_initTexture = false;
    return *this;
}

PartBase::~PartBase()
{
    if(m_pTexture)
        SDL_DestroyTexture(m_pTexture);
}

void PartBase::update(const CharaAction &actionName, const CharaDirection &direction, int deltaTime)
{
    const AnimationSequence* animationSequence = findAnimationSequence(m_spriteData, actionName, direction);
    if(animationSequence == nullptr || animationSequence->frames.empty())
        return;
    else if(1 == animationSequence->frames.size())
    {
        m_Frame = animationSequence->frames[0];
        return;
    }
       
    if (m_iFrameIndex >= static_cast<int>(animationSequence->frames.size())) {
        m_iFrameIndex = 0;
    }

    m_Frame = animationSequence->frames[m_iFrameIndex];
    m_iDeltaTime = m_iDeltaTime + deltaTime;

    if(m_iDeltaTime > m_Frame.delay * 2/3)
    {
        if(m_iFrameIndex + 1 < static_cast<int>(animationSequence->frames.size()))
            m_iFrameIndex++;
        else
        {
            if(!animationSequence->loop)
            {
                m_iFrameIndex = 100;
                m_Frame = animationSequence->frames[0];
            }
            else
            {
                m_iFrameIndex = 0;
                m_Frame = animationSequence->frames[m_iFrameIndex];
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
    const AnimationSequence* animationSequence = findAnimationSequence(m_spriteData, actionName, direction);
    if(animationSequence != nullptr && !animationSequence->frames.empty())
    {
        m_iFrameIndex = 0;
        m_Frame = animationSequence->frames[0];
    }
}

void PartBase::render(SDL_Renderer *renderer, const CharaAction &actionName, CharaDirection &dir, int &x, int &y)
{
    if(nullptr == m_pTexture)
        return;

    int cols, rows;
    cols = rows = 0;
    SDL_QueryTexture(m_pTexture, NULL, NULL, &cols, &rows);

    int frameWidth, frameHeight;

    if(!m_imageSet.imageSetPath.empty()){
        frameWidth = m_imageSet.frameWidth;
        frameHeight = m_imageSet.frameHeight;
    }
    else{
        frameWidth = m_spriteData.frameWidth;
        frameHeight = m_spriteData.frameHeight;
    }

    cols /= frameWidth;
    rows /= frameHeight;
    
    const AnimationSequence* animationSequence = findAnimationSequence(m_spriteData, actionName, dir);
    Frame renderFrame = m_Frame;
    if (animationSequence != nullptr && !animationSequence->frames.empty()) {
        int renderIndex = m_iFrameIndex;
        if (renderIndex < 0 || renderIndex >= static_cast<int>(animationSequence->frames.size())) {
            renderIndex = 0;
        }
        renderFrame = animationSequence->frames[renderIndex];
    }

    // int frame_x = (renderFrame.index % cols) * m_neutralImage.frameWidth;
    // int frame_y = (renderFrame.index % rows) * m_neutralImage.frameHeight;
    const int resolvedFrameIndex = resolveFrameIndex(*this, renderFrame);
    int frame_x = (resolvedFrameIndex % cols) * frameWidth;
    int frame_y = (resolvedFrameIndex / cols) * frameHeight;

    // 源矩形和目标矩形
    SDL_Rect src_rect = {frame_x, frame_y, frameWidth, frameHeight};
    SDL_Rect dest_rect = renderRect(actionName, dir, x, y);
    // 渲染
    SDL_RenderCopy(renderer, m_pTexture, &src_rect, &dest_rect);
    // spdlog::info("file" + m_neutralImage.file
    //             + "  x:" + std::to_string(x)
    //             + "  y:" + std::to_string(y));
}

void PartBase::renderScaled(SDL_Renderer* renderer, const CharaAction& actionName,
                            CharaDirection& dir, int& x, int& y, float scale)
{
    if (nullptr == m_pTexture) {
        return;
    }

    int cols = 0;
    int rows = 0;
    SDL_QueryTexture(m_pTexture, NULL, NULL, &cols, &rows);

    int frameWidth = 0;
    int frameHeight = 0;
    if (!m_imageSet.imageSetPath.empty()) {
        frameWidth = m_imageSet.frameWidth;
        frameHeight = m_imageSet.frameHeight;
    } else {
        frameWidth = m_spriteData.frameWidth;
        frameHeight = m_spriteData.frameHeight;
    }

    cols /= frameWidth;
    rows /= frameHeight;

    const AnimationSequence* animationSequence = findAnimationSequence(m_spriteData, actionName, dir);
    Frame renderFrame = m_Frame;
    if (animationSequence != nullptr && !animationSequence->frames.empty()) {
        int renderIndex = m_iFrameIndex;
        if (renderIndex < 0 || renderIndex >= static_cast<int>(animationSequence->frames.size())) {
            renderIndex = 0;
        }
        renderFrame = animationSequence->frames[renderIndex];
    }

    const int resolvedFrameIndex = resolveFrameIndex(*this, renderFrame);
    int frame_x = (resolvedFrameIndex % cols) * frameWidth;
    int frame_y = (resolvedFrameIndex / cols) * frameHeight;

    SDL_Rect src_rect = {frame_x, frame_y, frameWidth, frameHeight};
    SDL_Rect dest_rect = renderRect(actionName, dir, x, y);

    dest_rect.x = static_cast<int>(x + (dest_rect.x - x) * scale);
    dest_rect.y = static_cast<int>(y + (dest_rect.y - y) * scale);
    dest_rect.w = static_cast<int>(dest_rect.w * scale);
    dest_rect.h = static_cast<int>(dest_rect.h * scale);

    SDL_RenderCopy(renderer, m_pTexture, &src_rect, &dest_rect);
}

SDL_Rect PartBase::renderRect(const CharaAction& actionName, const CharaDirection& dir, int x, int y) const
{
    const AnimationSequence* animationSequence = findAnimationSequence(m_spriteData, actionName, dir);
    Frame renderFrame = m_Frame;
    if (animationSequence != nullptr && !animationSequence->frames.empty()) {
        int renderIndex = m_iFrameIndex;
        if (renderIndex < 0 || renderIndex >= static_cast<int>(animationSequence->frames.size())) {
            renderIndex = 0;
        }
        renderFrame = animationSequence->frames[renderIndex];
    }

    int frameWidth = 0;
    int frameHeight = 0;
    if (!m_imageSet.imageSetPath.empty()) {
        frameWidth = m_imageSet.frameWidth;
        frameHeight = m_imageSet.frameHeight;
    } else {
        frameWidth = m_spriteData.frameWidth;
        frameHeight = m_spriteData.frameHeight;
    }

    // TMWA 的部件偏移是围绕 64x64 人物基准框的“底部”来组织的：
    // 身体、头饰和 128x128 武器都应先对齐到底边，再叠加 XML 里的 offset。
    return SDL_Rect{x + (64 - frameWidth) / 2 + renderFrame.offsetX,
                    y + (64 - frameHeight) + renderFrame.offsetY,
                    frameWidth,
                    frameHeight};
}

bool PartBase::handleEvent(const SDL_Event &e, const CharaAction &actionName, const CharaDirection &direction)
{
    return false;
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
    if(m_initTexture)
        return;

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
    m_initTexture = true;

    // spdlog::info(pngPath);

}

void PartBase::setImageSet(const ImageSet &imageSet)
{
    m_imageSet = imageSet;
}

ImageSet PartBase::imageSet()
{
    return m_imageSet;
}

const ImageSet& PartBase::imageSet() const
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

const SpriteData& PartBase::spriteData() const
{
    return m_spriteData;
}

void PartBase::setVariant(int variant)
{
    m_variant = variant;
}

int PartBase::variant() const
{
    return m_variant;
}
