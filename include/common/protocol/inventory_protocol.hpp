#pragma once

#include <string>
#include <vector>
#include "json.hpp"

using json = nlohmann::json;

struct InventoryItem
{
    int itemId = 0;
    std::string name;
    int count = 0;
    std::string spritePath;
    std::string itemType = "material";
    std::string weaponType;
    int power = 0;
};

inline void to_json(json& js, const InventoryItem& item)
{
    js = json::object();
    js["itemId"] = item.itemId;
    js["name"] = item.name;
    js["count"] = item.count;
    js["spritePath"] = item.spritePath;
    js["itemType"] = item.itemType;
    js["weaponType"] = item.weaponType;
    js["power"] = item.power;
}

inline void from_json(const json& js, InventoryItem& item)
{
    item.itemId = js.value("itemId", 0);
    item.name = js.value("name", "");
    item.count = js.value("count", 0);
    item.spritePath = js.value("spritePath", "");
    item.itemType = js.value("itemType", "material");
    item.weaponType = js.value("weaponType", "");
    item.power = js.value("power", 0);
}

struct InventorySnapshot
{
    std::string playerUuid;
    std::vector<InventoryItem> items;
};

inline void to_json(json& js, const InventorySnapshot& snapshot)
{
    js = json::object();
    js["playerUuid"] = snapshot.playerUuid;
    js["items"] = snapshot.items;
}

inline void from_json(const json& js, InventorySnapshot& snapshot)
{
    snapshot.playerUuid = js.value("playerUuid", "");
    if (js.contains("items") && js["items"].is_array()) {
        snapshot.items = js["items"].get<std::vector<InventoryItem>>();
    } else {
        snapshot.items.clear();
    }
}
