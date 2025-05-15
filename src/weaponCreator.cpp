#include "weaponCreator.h"
#include <iostream>
#include <limits>
#include <algorithm>
#include <fstream>
#include <string>

std::string sanitizeWeaponNameToFilename(const std::string& weaponName) {
    std::string result;
    for (char ch : weaponName) {
        if (isalnum(ch)) {
            result += tolower(ch);
        }
        else if (isspace(ch)) {
            result += '_';
        }
    }
    if (result.empty()) result = "weapon";
    return result + ".txt";
}

namespace {
    std::string toLower(const std::string& str) {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(), ::tolower);
        return result;
    }

    int getValidChoice(size_t max_options, const std::string& prompt) {
        int choice = -1;
        while (true) {
            std::cout << prompt;
            if (!(std::cin >> choice)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input. Please enter a number.\n";
                continue;
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (choice > 0 && static_cast<size_t>(choice) <= max_options) {
                return choice - 1;
            }
            std::cout << "Please enter a number between 1 and " << max_options << ".\n";
        }
    }
}

void initializeDefaultParts(PartDatabase& db) {
    // Initialize with some default parts

    // Receivers
    Part standardReceiver{ "Standard Receiver", PartType::RECEIVER, 1, 6, 0, 50, 2.0f, 100.0f, 0xFF, 0 };
    Part heavyReceiver{ "Heavy Receiver", PartType::RECEIVER,  2, 8, 5, 30, 5, 200, 0x0F };
    Part lightReceiver{  "Light Receiver", PartType::RECEIVER, 1, 4, -5, 80, 1, 150, 0xF0 };

    db.addPart(standardReceiver);
    db.addPart(heavyReceiver);
    db.addPart(lightReceiver);

    // Barrels
    Part shortBarrel{  "Short Barrel",PartType::BARREL, 0, 0, -10, -5, -1, 50, 0 };
    Part standardBarrel{ "Standard Barrel", PartType::BARREL, 0, 1, 0, 10, 0, 75, 0 };
    Part longBarrel{  "Long Barrel",PartType::BARREL, 0, 0, 10, 40, 1, 100, 0 };

    db.addPart(shortBarrel);
    db.addPart(standardBarrel);
    db.addPart(longBarrel);

    // Sights
    Part ironSights{ "Iron Sights", PartType::SIGHT, 0, 0, 0, 0, 0, 25, 0 };
    Part reflex{  "Reflex Sight",PartType::SIGHT, 1, 0, 0, 5, 0, 50, 0 };
    Part scope{ "Scope", PartType::SIGHT, 0, 0, 5, 20, 1, 100, 0 };

    db.addPart(ironSights);
    db.addPart(reflex);
    db.addPart(scope);
}

Weapon* handleCreateWeapon(const std::string& name, PartDatabase& db) {
    // Check if database has necessary parts
    if (db.getParts(PartType::RECEIVER).empty() ||
        db.getParts(PartType::BARREL).empty() ||
        db.getParts(PartType::SIGHT).empty()) {
        return nullptr;
    }

    Weapon* newWeapon = new Weapon();
    newWeapon->name = name;

    // Select Receiver
    const auto& receivers = db.getParts(PartType::RECEIVER);
    std::cout << "\nSelect receiver type:\n";
    for (size_t i = 0; i < receivers.size(); ++i) {
        std::cout << i + 1 << ". " << receivers[i].name << "\n";
    }
    int receiverIdx = getValidChoice(receivers.size(), "Enter choice: ");
    const Part& receiver = receivers[receiverIdx];
    newWeapon->category = receiver.compatibilityMask;
    newWeapon->damageQuantity = receiver.hitDieMod;
    newWeapon->damageQuality = receiver.dmgDieMod;
    newWeapon->rangeMin = receiver.rangeMinMod;
    newWeapon->rangeMax = receiver.rangeMaxMod;
    newWeapon->weight = receiver.weightMod;
    newWeapon->value = receiver.valueMod;

    // Select Barrel
    const auto& barrels = db.getParts(PartType::BARREL);
    std::cout << "\nSelect barrel type:\n";
    for (size_t i = 0; i < barrels.size(); ++i) {
        std::cout << i + 1 << ". " << barrels[i].name << "\n";
    }
    int barrelIdx = getValidChoice(barrels.size(), "Enter choice: ");
    const Part& barrel = barrels[barrelIdx];
    newWeapon->damageQuantity += barrel.hitDieMod;
    newWeapon->damageQuality += barrel.dmgDieMod;
    newWeapon->rangeMin += barrel.rangeMinMod;
    newWeapon->rangeMax += barrel.rangeMaxMod;
    newWeapon->weight += barrel.weightMod;
    newWeapon->value += barrel.valueMod;

    // Select Sight
    const auto& sights = db.getParts(PartType::SIGHT);
    std::cout << "\nSelect sight type:\n";
    for (size_t i = 0; i < sights.size(); ++i) {
        std::cout << i + 1 << ". " << sights[i].name << "\n";
    }
    int sightIdx = getValidChoice(sights.size(), "Enter choice: ");
    const Part& sight = sights[sightIdx];
    newWeapon->damageQuantity += sight.hitDieMod;
    newWeapon->damageQuality += sight.dmgDieMod;
    newWeapon->rangeMin += sight.rangeMinMod;
    newWeapon->rangeMax += sight.rangeMaxMod;
    newWeapon->weight += sight.weightMod;
    newWeapon->value += sight.valueMod;

    // Add Note or Flavor
    std::cout << "\nEnter note or flavor (optional):\n";
    std::getline(std::cin, newWeapon->noteOrFlavor);

    // Final adjustments
    newWeapon->damageQuantity = std::max(1, newWeapon->damageQuantity);
    newWeapon->damageQuality = std::clamp(newWeapon->damageQuality, 1, 6);
    newWeapon->rangeMin = std::max(0, newWeapon->rangeMin);
    newWeapon->rangeMax = std::max(newWeapon->rangeMin, std::min(300, newWeapon->rangeMax));

    // Display results
    std::cout << "\nWeapon created: " << newWeapon->name << "\n";
    std::cout << "Damage: " << newWeapon->damageQuantity << "d" << newWeapon->damageQuality << "\n";
    std::cout << "Range: " << newWeapon->rangeMin << " - " << newWeapon->rangeMax << "\n";
    std::cout << "Weight: " << newWeapon->weight << "\n";
    std::cout << "Value: " << newWeapon->value << "\n";

    return newWeapon;
}

bool promptSaveWeapon(const Weapon& weapon) {
    std::cout << "\nSave weapon to file? (y/n): ";
    char choice;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (tolower(choice) == 'y') {
        std::string filename = sanitizeWeaponNameToFilename(weapon.name);
        std::ofstream out(filename);
        if (out) {
            out << "Weapon: " << weapon.name << "\n"
                << "Damage: " << weapon.damageQuantity << "d" << weapon.damageQuality << "\n"
                << "Range: " << weapon.rangeMin << "-" << weapon.rangeMax << "\n"
                << "Weight: " << weapon.weight << "\n"
                << "Value: " << weapon.value << "\n"
                << "Note: " << weapon.noteOrFlavor << "\n";
            std::cout << "Saved to " << filename << "\n";
            return true;
        }
        std::cerr << "Failed to save weapon!\n";
    }
    return false;
}