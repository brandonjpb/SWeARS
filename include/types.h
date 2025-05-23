#pragma once
#include <string>
#include <map>
#include <cstdint>

// std::string sanitizeWeaponNameToFilename(const std::string& weaponName);
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

class Weapon {
public:
    Weapon(const std::string& name = "Unnamed Weapon");

    void applyPart(const Part& part);
    void setNote(const std::string& note);
    void describe() const;

    std::string getName() const;
    std::string getNote() const;

    int getDamageQuantity() const;
    int getDamageQuality() const;
    int getRangeMin() const;
    int getRangeMax() const;
    float getWeight() const;
    float getValue() const;
    uint32_t getCategory() const;

private:
    std::string name;
    std::string noteOrFlavor;
    uint32_t category = CATEGORY_NONE;
    int damageQuantity = 1;
    int damageQuality = 6;
    int rangeMin = 0;
    int rangeMax = 100;
    float weight = 1.0f;
    float value = 100.0f;
};


extern std::map<std::string, PartType> partTypeMap;
extern std::map<std::string, Category> categoryMap;