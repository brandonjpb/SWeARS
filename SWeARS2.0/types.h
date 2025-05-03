#pragma once
#include <string>
#include <map>
#include <cstdint>

std::string sanitizeWeaponNameToFilename(const std::string& weaponName);
std::string trim(const std::string& str);

enum class PartType { 
    RECEIVER, 
    BARREL, 
    SIGHT 
};

enum Category : uint32_t {
    CATEGORY_NONE = 0,
    CATEGORY_BALLISTIC = 1 << 0,
    CATEGORY_PLASMA = 1 << 1,
    CATEGORY_LASER = 1 << 2,
    CATEGORY_HEAVY = 1 << 3,
    CATEGORY_ALL = 0xFFFFFFFF
};

struct Part {
    std::string name;
    PartType type;
    int hitDieMod;
    int dmgDieMod;
    int rangeMinMod;
    int rangeMaxMod;
    float weightMod;
    float valueMod;
    uint32_t compatibilityMask;
    uint32_t exclusionMask;
};

struct Weapon {
    std::string name;
    std::string noteOrFlavor;
    uint32_t category;
    int damageQuantity;
    int damageQuality;
    int rangeMin;
    int rangeMax;
    float weight;
    float value;
};

extern std::map<std::string, PartType> partTypeMap;
extern std::map<std::string, Category> categoryMap;