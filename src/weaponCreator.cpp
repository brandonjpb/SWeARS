#include "weaponCreator.h"
#include "types.h"
#include <iostream>
#include <limits>
#include <algorithm>
#include <fstream>
#include <string>
#include <memory>

WeaponBuilder::WeaponBuilder(const std::string& name) {
    weapon = std::make_unique<Weapon>(name);
}

void WeaponBuilder::addPart(const Part& part) {
    weapon->applyPart(part);
}

void WeaponBuilder::setNote(const std::string& note) {
    weapon->setNote(note);
}

std::unique_ptr<Weapon> WeaponBuilder::build() {
    return std::move(weapon);
}

std::unique_ptr<Weapon> handleCreateWeapon(const std::string& name, PartDatabase& db) {
    if (db.getParts(PartType::RECEIVER).empty() ||
        db.getParts(PartType::BARREL).empty() ||
        db.getParts(PartType::SIGHT).empty()) {
        return nullptr;
    }

    WeaponBuilder builder(name);

    auto selectPart = [](PartType type, const std::vector<Part>& parts) -> const Part& {
        std::cout << "\nSelect " << (type == PartType::RECEIVER ? "receiver" :
            type == PartType::BARREL ? "barrel" : "sight") << ":\n";
        for (size_t i = 0; i < parts.size(); ++i) {
            std::cout << i + 1 << ". " << parts[i].name << "\n";
        }

        int index = -1;
        while (true) {
            std::cout << "Enter choice: ";
            if (!(std::cin >> index)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input. Try again.\n";
                continue;
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (index > 0 && static_cast<size_t>(index) <= parts.size()) break;
            std::cout << "Please choose between 1 and " << parts.size() << "\n";
        }
        return parts[index - 1];
        };

    const Part& receiver = selectPart(PartType::RECEIVER, db.getParts(PartType::RECEIVER));
    builder.addPart(receiver);

    const Part& barrel = selectPart(PartType::BARREL, db.getParts(PartType::BARREL));
    builder.addPart(barrel);

    const Part& sight = selectPart(PartType::SIGHT, db.getParts(PartType::SIGHT));
    builder.addPart(sight);

    std::cout << "\nEnter note or flavor (optional):\n";
    std::string note;
    std::getline(std::cin, note);
    builder.setNote(note);

    auto weapon = builder.build();
    weapon->describe();
    return weapon;
}

// === Fallback for initializing parts ===
void initializeDefaultParts(PartDatabase& db) {
    // Receivers
    db.addPart({ "Ballistic Receiver", PartType::RECEIVER, 1, 2, 60, 180, 4, 170, CATEGORY_BALLISTIC, CATEGORY_NONE });
    db.addPart({ "Plasma Receiver", PartType::RECEIVER, 2, 2, 60, 120, 7, 225, CATEGORY_PLASMA, CATEGORY_NONE });
    db.addPart({ "Laser Receiver", PartType::RECEIVER, 3, 1, 20, 180, 5, 420, CATEGORY_LASER, CATEGORY_NONE });
    db.addPart({ "Heavy Receiver", PartType::RECEIVER, 4, 2, 60, 120, 14, 600, CATEGORY_HEAVY, CATEGORY_NONE });

    // Barrels
    db.addPart({ "Short Barrel", PartType::BARREL, 0, 0, -20, -60, -0.5f, 0, CATEGORY_ALL, CATEGORY_NONE });
    db.addPart({ "Standard Barrel", PartType::BARREL, 0, 0, 0, 0, 0.0f, 0, CATEGORY_ALL, CATEGORY_NONE });
    db.addPart({ "Long Barrel", PartType::BARREL, 0, 0, 20, 60, 1.2f, 25, CATEGORY_ALL, CATEGORY_NONE });

    // Sights
    db.addPart({ "Iron Sight", PartType::SIGHT, 0, 0, 0, 0, 0.0f, 0, CATEGORY_ALL, CATEGORY_NONE });
    db.addPart({ "4x Scope", PartType::SIGHT, 0, 0, 120, 240, 2.0f, 130, CATEGORY_ALL, CATEGORY_HEAVY });
}

bool promptSaveWeapon(const Weapon& weapon) {
    std::cout << "\nSave weapon to file? (y/n): ";
    char choice;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (tolower(choice) == 'y') {
        std::string filename = /*sanitizeWeaponNameToFilename*/(weapon.getName());
        std::ofstream out(filename);
        if (out) {
            out << "Weapon: " << weapon.getName() << "\n"
                << "Damage: " << weapon.getDamageQuantity() << "d" << weapon.getDamageQuality() << "\n"
                << "Range: " << weapon.getRangeMin() << "-" << weapon.getRangeMax() << "\n"
                << "Weight: " << weapon.getWeight() << "\n"
                << "Value: " << weapon.getValue() << "\n"
                << "Note: " << weapon.getNote() << "\n";
            std::cout << "Saved to " << filename << "\n";
            return true;
        }
        std::cerr << "Failed to save weapon!\n";
    }
    return false;
}
