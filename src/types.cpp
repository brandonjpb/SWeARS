#include "types.h"
#include <iostream>
#include <algorithm>

Weapon::Weapon(const std::string& name)
    : name(name) {
}

void Weapon::applyPart(const Part& part) {
    // Only receivers set these
    if (part.type == PartType::RECEIVER) {
        category = part.compatibilityMask;
        damageQuantity = part.hitDieMod;
        damageQuality = part.dmgDieMod;
        rangeMin = part.rangeMinMod;
        rangeMax = part.rangeMaxMod;
        weight = part.weightMod;
        value = part.valueMod;
    }
    else {
        damageQuantity += part.hitDieMod;
        damageQuality += part.dmgDieMod;
        rangeMin += part.rangeMinMod;
        rangeMax += part.rangeMaxMod;
        weight += part.weightMod;
        value += part.valueMod;
    }

    // Clamp final stats
    damageQuantity = std::max(1, damageQuantity);
    damageQuality = std::clamp(damageQuality, 1, 6);
    rangeMin = std::max(0, rangeMin);
    rangeMax = std::max(rangeMin, std::min(300, rangeMax));
}

void Weapon::setNote(const std::string& note) {
    noteOrFlavor = note;
}

void Weapon::describe() const {
    std::cout << "\nWeapon: " << name << "\n"
        << "Damage: " << damageQuantity << "d" << damageQuality << "\n"
        << "Range: " << rangeMin << "-" << rangeMax << "\n"
        << "Weight: " << weight << "\n"
        << "Value: " << value << "\n"
        << "Note: " << noteOrFlavor << "\n";
}

std::string Weapon::getName() const { return name; }
std::string Weapon::getNote() const { return noteOrFlavor; }
int Weapon::getDamageQuantity() const { return damageQuantity; }
int Weapon::getDamageQuality() const { return damageQuality; }
int Weapon::getRangeMin() const { return rangeMin; }
int Weapon::getRangeMax() const { return rangeMax; }
float Weapon::getWeight() const { return weight; }
float Weapon::getValue() const { return value; }
uint32_t Weapon::getCategory() const { return category; }
