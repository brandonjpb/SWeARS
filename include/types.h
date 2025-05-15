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
    PartType type = PartType::RECEIVER;
    int hitDieMod = 0;
    int dmgDieMod = 0;
    int rangeMinMod = 0;
    int rangeMaxMod = 0;
    float weightMod = 0.0f;
    float valueMod = 0.0f;
    uint32_t compatibilityMask = 0;
    uint32_t exclusionMask = 0;
};

struct Weapon {
    std::string name;
    std::string noteOrFlavor;
    uint32_t category = CATEGORY_NONE;  // Initialize with default
    int damageQuantity = 1;             // Default to 1
    int damageQuality = 6;              // Default to d6
    int rangeMin = 0;                   // Default 0 range
    int rangeMax = 100;                 // Default 100 range
    float weight = 1.0f;                // Default 1.0 kg
    float value = 100.0f;               // Default 100 currency
};

extern std::map<std::string, PartType> partTypeMap;
extern std::map<std::string, Category> categoryMap;