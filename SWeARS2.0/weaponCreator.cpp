#include "weaponCreator.h"
#include <iostream>
#include <limits>
#include <algorithm>
#include <fstream>

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

void handleCreateWeapon() {
    try {
        Weapon newWeapon{};
        std::cout << "Enter weapon name: ";
        std::getline(std::cin, newWeapon.name);

        // Select Receiver
        const auto& receivers = partDB.getParts(PartType::RECEIVER);
        std::cout << "\nSelect receiver type:\n";
        for (size_t i = 0; i < receivers.size(); ++i) {
            std::cout << i + 1 << ". " << receivers[i].name << "\n";
        }
        int receiverIdx = getValidChoice(receivers.size(), "Enter choice: ");
        const Part& receiver = receivers[receiverIdx];
        newWeapon.category = receiver.compatibilityMask;
        newWeapon.damageQuantity = receiver.hitDieMod;
        newWeapon.damageQuality = receiver.dmgDieMod;
        newWeapon.rangeMin = receiver.rangeMinMod;
        newWeapon.rangeMax = receiver.rangeMaxMod;
        newWeapon.weight = receiver.weightMod;
        newWeapon.value = receiver.valueMod;

        // Select Barrel
        const auto& barrels = partDB.getParts(PartType::BARREL);
        std::cout << "\nSelect barrel type:\n";
        for (size_t i = 0; i < barrels.size(); ++i) {
            std::cout << i + 1 << ". " << barrels[i].name << "\n";
        }
        int barrelIdx = getValidChoice(barrels.size(), "Enter choice: ");
        const Part& barrel = barrels[barrelIdx];
        newWeapon.damageQuantity += barrel.hitDieMod;
        newWeapon.damageQuality += barrel.dmgDieMod;
        newWeapon.rangeMin += barrel.rangeMinMod;
        newWeapon.rangeMax += barrel.rangeMaxMod;
        newWeapon.weight += barrel.weightMod;
        newWeapon.value += barrel.valueMod;

        // Select Sight
        const auto& sights = partDB.getParts(PartType::SIGHT);
        std::cout << "\nSelect sight type:\n";
        for (size_t i = 0; i < sights.size(); ++i) {
            std::cout << i + 1 << ". " << sights[i].name << "\n";
        }
        int sightIdx = getValidChoice(sights.size(), "Enter choice: ");
        const Part& sight = sights[sightIdx];
        newWeapon.damageQuantity += sight.hitDieMod;
        newWeapon.damageQuality += sight.dmgDieMod;
        newWeapon.rangeMin += sight.rangeMinMod;
        newWeapon.rangeMax += sight.rangeMaxMod;
        newWeapon.weight += sight.weightMod;
        newWeapon.value += sight.valueMod;

        // Add Note or Flavor
        std::cout << "\nEnter note or flavor (optional):\n";
        std::cin >> newWeapon.noteOrFlavor;

        // Final adjustments
        newWeapon.damageQuantity = std::max(1, newWeapon.damageQuantity);
        newWeapon.damageQuality = std::clamp(newWeapon.damageQuality, 1, 6);
        newWeapon.rangeMin = std::max(0, newWeapon.rangeMin);
        newWeapon.rangeMax = std::min(300, newWeapon.rangeMax);

        // Display results
        std::cout << "\nWeapon created: " << newWeapon.name << "\n";
        std::cout << "Damage: " << newWeapon.damageQuantity << "d" << newWeapon.damageQuality << "\n";
        std::cout << "Range: " << newWeapon.rangeMin << " - " << newWeapon.rangeMax << "\n";
        std::cout << "Weight: " << newWeapon.weight << "\n";
        std::cout << "Value: " << newWeapon.value << "\n";

        while (true) {
            std::cout << "\nSave weapon to file? (y/n): ";
            char choice;
            std::cin >> choice;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            choice = tolower(choice);
            if (choice == 'y' || choice == 'n') {
                if (choice == 'y') {
                    std::string filename = sanitizeWeaponNameToFilename(newWeapon.name);
                    std::ofstream out(filename);
                    if (out) {
                        out << "Weapon: " << newWeapon.name << "\n"
                            << "Damage: " << newWeapon.damageQuantity << "d" << newWeapon.damageQuality << "\n"
                            << "Range: " << newWeapon.rangeMin << "-" << newWeapon.rangeMax << "\n"
                            << "Weight: " << newWeapon.weight << "\n"
                            << "Value: " << newWeapon.value << "\n"
                            << "Note: " << newWeapon.noteOrFlavor << "\n";
                        std::cout << "Saved to " << filename << "\n";
                    }
                    else {
                        std::cerr << "Failed to save weapon file!\n";
                    }
                }
                break;
            }
            std::cout << "Please enter 'y' or 'n': ";
        }
    }

    catch (const std::exception& e) {
        std::cerr << "Error creating weapon: " << e.what() << "\n";
    }
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
