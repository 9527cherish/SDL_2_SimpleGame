#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <optional>
#include <string>
#include <vector>
#include "messageInfo.hpp"

class InventoryManager
{
public:
    ~InventoryManager();
    static InventoryManager& getInstance();

    void syncInventory(const InventorySnapshot& snapshot);
    void clear();
    void togglePanel();
    bool panelVisible() const;
    bool handleEvent(const SDL_Event& e);
    void renderPanel(SDL_Renderer* renderer);
    std::optional<InventoryItem> equippedWeapon() const;

private:
    InventoryManager() = default;
    InventoryManager(const InventoryManager&) = delete;
    InventoryManager& operator=(const InventoryManager&) = delete;

    void ensureFont();
    std::string summaryText() const;
    std::string localizedName(const InventoryItem& item) const;
    std::string localizedEffect(const InventoryItem& item) const;
    void renderTooltip(SDL_Renderer* renderer, const InventoryItem& item, int mouseX, int mouseY);
    bool isWeapon(const InventoryItem& item) const;
    void syncEquippedWeapon();

private:
    std::vector<InventoryItem> m_items;
    bool m_panelVisible = true;
    TTF_Font* m_font = nullptr;
    int m_equippedItemId = -1;
};
