#include "Inventory/inventoryManager.hpp"

#include <SDL2/SDL_image.h>
#include <array>
#include <algorithm>
#include <map>
#include <spdlog/spdlog.h>

namespace {
std::array<std::string, 3> kFontCandidates{
    "SourceHanSansCN-Regular.otf",
    "bin/SourceHanSansCN-Regular.otf",
    "../bin/SourceHanSansCN-Regular.otf"
};

struct ItemTextInfo
{
    const char* chineseName;
    const char* effect;
};

const std::map<std::string, ItemTextInfo> kItemTextMap{
    {"Slime Jelly", {"史莱姆果冻", "黏糊糊的基础素材，可用于制作低阶恢复品。"}},
    {"Bee Stinger", {"蜂针", "尖锐的采集材料，常用于轻型箭矢或毒性配方。"}},
    {"Bat Wing", {"蝙蝠翅", "轻薄的飞行素材，可作为敏捷系装备的辅料。"}},
    {"Bone Darts", {"骨制飞镖", "简陋但锋利的投掷物，提高远程消耗能力。"}},
    {"Dark Talisman", {"暗影护符", "带有阴影气息的护符，适合偏法术或诅咒用途。"}},
    {"Pinkie Helmet", {"粉咩头盔", "造型可爱的头盔，提供少量防护。"}},
    {"Fourleaf Amulet", {"四叶护符", "象征幸运的小护符，适合冒险时随身携带。"}},
    {"Short Sword", {"短剑", "轻便近战武器，刺击速度较快。"}},
    {"Hunter Bow", {"猎手短弓", "发射箭矢的远程武器，适合拉开距离输出。"}},
    {"Apprentice Wand", {"学徒魔杖", "释放基础魔法弹，命中时附带奥术冲击。"}},
    {"Iron Spear", {"铁枪", "攻击距离更长，适合直线突刺。"}},
    {"Battle Axe", {"战斧", "沉重但有力的近战武器，挥砍伤害较高。"}}
};
}

InventoryManager::~InventoryManager()
{
    if (m_font != nullptr) {
        TTF_CloseFont(m_font);
        m_font = nullptr;
    }
}

InventoryManager& InventoryManager::getInstance()
{
    static InventoryManager instance;
    return instance;
}

void InventoryManager::syncInventory(const InventorySnapshot& snapshot)
{
    m_items = snapshot.items;
    syncEquippedWeapon();
}

void InventoryManager::clear()
{
    m_items.clear();
    m_equippedItemId = -1;
}

void InventoryManager::togglePanel()
{
    m_panelVisible = !m_panelVisible;
}

bool InventoryManager::panelVisible() const
{
    return m_panelVisible;
}

bool InventoryManager::isWeapon(const InventoryItem& item) const
{
    return item.itemType == "weapon";
}

void InventoryManager::syncEquippedWeapon()
{
    if (m_items.empty()) {
        m_equippedItemId = -1;
        return;
    }

    auto iter = std::find_if(m_items.begin(), m_items.end(), [&](const InventoryItem& item) {
        return item.itemId == m_equippedItemId;
    });
    if (iter != m_items.end()) {
        return;
    }

    auto weaponIter = std::find_if(m_items.begin(), m_items.end(), [&](const InventoryItem& item) {
        return isWeapon(item);
    });
    m_equippedItemId = weaponIter != m_items.end() ? weaponIter->itemId : -1;
}

bool InventoryManager::handleEvent(const SDL_Event& e)
{
    if (!m_panelVisible || e.type != SDL_MOUSEBUTTONDOWN || e.button.button != SDL_BUTTON_LEFT) {
        return false;
    }

    const SDL_Point mousePoint{e.button.x, e.button.y};
    SDL_Rect panelRect{18, 62, 360, 212};
    if (!SDL_PointInRect(&mousePoint, &panelRect)) {
        return false;
    }

    const int slotSize = 52;
    const int columns = 5;
    for (int i = 0; i < 10 && i < static_cast<int>(m_items.size()); ++i)
    {
        const int row = i / columns;
        const int col = i % columns;
        SDL_Rect slotRect{
            panelRect.x + 14 + col * (slotSize + 12),
            panelRect.y + 16 + row * (slotSize + 34),
            slotSize,
            slotSize
        };

        if (!SDL_PointInRect(&mousePoint, &slotRect)) {
            continue;
        }

        if (isWeapon(m_items[i])) {
            m_equippedItemId = m_items[i].itemId;
        }
        return true;
    }

    return false;
}

void InventoryManager::ensureFont()
{
    if (m_font != nullptr) {
        return;
    }

    if (!TTF_WasInit() && TTF_Init() == -1) {
        spdlog::error("初始化背包字体失败: {}", TTF_GetError());
        return;
    }

    for (const std::string& fontPath : kFontCandidates)
    {
        m_font = TTF_OpenFont(fontPath.c_str(), 14);
        if (m_font != nullptr) {
            return;
        }
    }

    spdlog::error("加载背包字体失败: {}", TTF_GetError());
}

std::string InventoryManager::summaryText() const
{
    int totalCount = 0;
    for (const InventoryItem& item : m_items)
    {
        totalCount += item.count;
    }
    return "背包(B) 种类: " + std::to_string(m_items.size()) +
           " 总数: " + std::to_string(totalCount) +
           "  左键点击武器可装备";
}

std::string InventoryManager::localizedName(const InventoryItem& item) const
{
    auto iter = kItemTextMap.find(item.name);
    if (iter != kItemTextMap.end()) {
        return iter->second.chineseName;
    }
    return item.name;
}

std::string InventoryManager::localizedEffect(const InventoryItem& item) const
{
    auto iter = kItemTextMap.find(item.name);
    if (iter != kItemTextMap.end()) {
        return iter->second.effect;
    }
    if (isWeapon(item))
    {
        if (item.weaponType == "bow") {
            return "装备后攻击会切换为远程射箭，并提升基础伤害。威力 +" + std::to_string(item.power);
        }
        if (item.weaponType == "wand") {
            return "装备后可释放魔法弹，攻击动作会切换为施法。威力 +" + std::to_string(item.power);
        }
        if (item.weaponType == "spear") {
            return "装备后攻击会切换为突刺，适合中距离近战。威力 +" + std::to_string(item.power);
        }
        if (item.weaponType == "axe") {
            return "装备后攻击会切换为重劈，拥有更高单次伤害。威力 +" + std::to_string(item.power);
        }
        if (item.weaponType == "sword") {
            return "装备后攻击会切换为剑刺，攻速与伤害较为均衡。威力 +" + std::to_string(item.power);
        }
    }
    return "暂未记录该物品效果，可作为后续合成或装备素材使用。";
}

std::optional<InventoryItem> InventoryManager::equippedWeapon() const
{
    auto iter = std::find_if(m_items.begin(), m_items.end(), [&](const InventoryItem& item) {
        return item.itemId == m_equippedItemId && isWeapon(item);
    });
    if (iter == m_items.end()) {
        return std::nullopt;
    }
    return *iter;
}

void InventoryManager::renderTooltip(SDL_Renderer* renderer, const InventoryItem& item, int mouseX, int mouseY)
{
    if (renderer == nullptr || m_font == nullptr) {
        return;
    }

    const std::string title = localizedName(item);
    const std::string effect = localizedEffect(item);
    SDL_Color titleColor{246, 240, 200, 255};
    SDL_Color bodyColor{232, 238, 234, 255};

    SDL_Surface* titleSurface = TTF_RenderUTF8_Blended(m_font, title.c_str(), titleColor);
    SDL_Surface* effectSurface = TTF_RenderUTF8_Blended_Wrapped(m_font, effect.c_str(), bodyColor, 220);
    if (titleSurface == nullptr || effectSurface == nullptr)
    {
        if (titleSurface != nullptr) SDL_FreeSurface(titleSurface);
        if (effectSurface != nullptr) SDL_FreeSurface(effectSurface);
        return;
    }

    const int tooltipWidth = std::max(titleSurface->w, effectSurface->w) + 20;
    const int tooltipHeight = titleSurface->h + effectSurface->h + 22;
    SDL_Rect tooltipRect{mouseX + 14, mouseY + 14, tooltipWidth, tooltipHeight};
    const int maxX = 800 - tooltipRect.w - 8;
    const int maxY = 600 - tooltipRect.h - 8;
    tooltipRect.x = std::clamp(tooltipRect.x, 8, std::max(8, maxX));
    tooltipRect.y = std::clamp(tooltipRect.y, 8, std::max(8, maxY));

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 18, 22, 28, 235);
    SDL_RenderFillRect(renderer, &tooltipRect);
    SDL_SetRenderDrawColor(renderer, 170, 184, 132, 255);
    SDL_RenderDrawRect(renderer, &tooltipRect);

    SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
    SDL_Texture* effectTexture = SDL_CreateTextureFromSurface(renderer, effectSurface);
    if (titleTexture != nullptr)
    {
        SDL_Rect titleRect{tooltipRect.x + 10, tooltipRect.y + 8, titleSurface->w, titleSurface->h};
        SDL_RenderCopy(renderer, titleTexture, nullptr, &titleRect);
        SDL_DestroyTexture(titleTexture);
    }
    if (effectTexture != nullptr)
    {
        SDL_Rect effectRect{tooltipRect.x + 10, tooltipRect.y + 12 + titleSurface->h, effectSurface->w, effectSurface->h};
        SDL_RenderCopy(renderer, effectTexture, nullptr, &effectRect);
        SDL_DestroyTexture(effectTexture);
    }

    SDL_FreeSurface(titleSurface);
    SDL_FreeSurface(effectSurface);
}

void InventoryManager::renderPanel(SDL_Renderer* renderer)
{
    if (renderer == nullptr) {
        return;
    }

    ensureFont();

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    SDL_Rect summaryRect{18, 18, 320, 34};
    SDL_SetRenderDrawColor(renderer, 10, 16, 20, 175);
    SDL_RenderFillRect(renderer, &summaryRect);
    SDL_SetRenderDrawColor(renderer, 120, 156, 138, 220);
    SDL_RenderDrawRect(renderer, &summaryRect);

    if (m_font != nullptr)
    {
        SDL_Color textColor{236, 240, 232, 255};
        std::string summary = summaryText();
        if (std::optional<InventoryItem> weapon = equippedWeapon()) {
            summary += "  装备: " + localizedName(*weapon);
        }
        SDL_Surface* summarySurface = TTF_RenderUTF8_Blended(m_font, summary.c_str(), textColor);
        if (summarySurface != nullptr)
        {
            SDL_Texture* summaryTexture = SDL_CreateTextureFromSurface(renderer, summarySurface);
            if (summaryTexture != nullptr)
            {
                SDL_Rect textRect{summaryRect.x + 10, summaryRect.y + 8, summarySurface->w, summarySurface->h};
                SDL_RenderCopy(renderer, summaryTexture, nullptr, &textRect);
                SDL_DestroyTexture(summaryTexture);
            }
            SDL_FreeSurface(summarySurface);
        }
    }

    if (!m_panelVisible) {
        return;
    }

    SDL_Rect panelRect{18, 62, 360, 212};
    SDL_SetRenderDrawColor(renderer, 12, 20, 24, 210);
    SDL_RenderFillRect(renderer, &panelRect);
    SDL_SetRenderDrawColor(renderer, 120, 156, 138, 220);
    SDL_RenderDrawRect(renderer, &panelRect);

    const int slotSize = 52;
    const int columns = 5;
    int mouseX = 0;
    int mouseY = 0;
    SDL_GetMouseState(&mouseX, &mouseY);
    const InventoryItem* hoveredItem = nullptr;
    for (int i = 0; i < 10; ++i)
    {
        const int row = i / columns;
        const int col = i % columns;
        SDL_Rect slotRect{
            panelRect.x + 14 + col * (slotSize + 12),
            panelRect.y + 16 + row * (slotSize + 34),
            slotSize,
            slotSize
        };
        SDL_SetRenderDrawColor(renderer, 26, 36, 40, 220);
        SDL_RenderFillRect(renderer, &slotRect);
        SDL_SetRenderDrawColor(renderer, 92, 118, 104, 220);
        SDL_RenderDrawRect(renderer, &slotRect);

        if (i >= static_cast<int>(m_items.size())) {
            continue;
        }

        const InventoryItem& item = m_items[i];
        if (mouseX >= slotRect.x && mouseX < slotRect.x + slotRect.w &&
            mouseY >= slotRect.y && mouseY < slotRect.y + slotRect.h)
        {
            hoveredItem = &item;
            SDL_SetRenderDrawColor(renderer, 168, 184, 108, 255);
            SDL_RenderDrawRect(renderer, &slotRect);
        }
        if (item.itemId == m_equippedItemId && isWeapon(item))
        {
            SDL_SetRenderDrawColor(renderer, 82, 164, 228, 255);
            SDL_RenderDrawRect(renderer, &slotRect);
        }

        SDL_Texture* icon = IMG_LoadTexture(renderer, item.spritePath.c_str());
        if (icon != nullptr)
        {
            SDL_Rect iconRect{slotRect.x + 6, slotRect.y + 6, slotRect.w - 12, slotRect.h - 12};
            SDL_RenderCopy(renderer, icon, nullptr, &iconRect);
            SDL_DestroyTexture(icon);
        }

        if (m_font != nullptr)
        {
            SDL_Color textColor{244, 244, 236, 255};
            const std::string countText = "x" + std::to_string(std::max(1, item.count));
            SDL_Surface* countSurface = TTF_RenderUTF8_Blended(m_font, countText.c_str(), textColor);
            if (countSurface != nullptr)
            {
                SDL_Texture* countTexture = SDL_CreateTextureFromSurface(renderer, countSurface);
                if (countTexture != nullptr)
                {
                    SDL_Rect countRect{
                        slotRect.x + slotRect.w - countSurface->w - 4,
                        slotRect.y + slotRect.h - countSurface->h - 2,
                        countSurface->w,
                        countSurface->h
                    };
                    SDL_RenderCopy(renderer, countTexture, nullptr, &countRect);
                    SDL_DestroyTexture(countTexture);
                }
                SDL_FreeSurface(countSurface);
            }

            const std::string displayName = localizedName(item);
            SDL_Surface* nameSurface = TTF_RenderUTF8_Blended(m_font, displayName.c_str(), textColor);
            if (nameSurface != nullptr)
            {
                SDL_Texture* nameTexture = SDL_CreateTextureFromSurface(renderer, nameSurface);
                if (nameTexture != nullptr)
                {
                    SDL_Rect nameRect{slotRect.x, slotRect.y + slotRect.h + 4, nameSurface->w, nameSurface->h};
                    SDL_RenderCopy(renderer, nameTexture, nullptr, &nameRect);
                    SDL_DestroyTexture(nameTexture);
                }
                SDL_FreeSurface(nameSurface);
            }
        }
    }

    if (hoveredItem != nullptr) {
        renderTooltip(renderer, *hoveredItem, mouseX, mouseY);
    }
}
