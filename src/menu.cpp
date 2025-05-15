#include "menu.h"
#include "partdb.h"
#include "weaponCreator.h"
#include <iostream>
#include <algorithm>
#include <limits>
#include <unordered_map>
#include <cctype>

// Helper function to convert string to lowercase
std::string toLower(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(),
        [](unsigned char c) { return std::tolower(c); });
    return result;
}

// Helper function to trim whitespace
std::string trim(const std::string& str) {
    const auto first = str.find_first_not_of(" \t");
    if (first == std::string::npos) return "";
    const auto last = str.find_last_not_of(" \t");
    return str.substr(first, (last - first + 1));
}

void displayMenu() {
    std::cout << "\n=== WEAPON CREATOR ===\n";
    std::cout << "1. Search for a Part (or 'search')\n";
    std::cout << "2. List all Parts (or 'list')\n";
    std::cout << "3. Add a Part (or 'add')\n";
    std::cout << "4. Delete a Part (or 'delete')\n";
    std::cout << "5. Create a Weapon (or 'create')\n";
    std::cout << "6. Save Parts (or 'save')\n";
    std::cout << "7. Exit (or 'exit')\n";
    std::cout << "Choose an option: ";
}

MenuOption getMenuChoice() {
    while (true) {
        std::string input;
        std::getline(std::cin, input);
        input = toLower(trim(input));

        if (input.empty()) continue;

        // Check numeric input
        if (input.size() == 1 && isdigit(input[0])) {
            int choice = input[0] - '0';
            if (choice >= 1 && choice <= 7) {
                return static_cast<MenuOption>(choice - 1);
            }
        }

        // Check text input
        static const std::unordered_map<std::string, MenuOption> textOptions = {
            {"search", MenuOption::SEARCH_PART},
            {"list", MenuOption::LIST_PARTS},
            {"add", MenuOption::ADD_PART},
            {"delete", MenuOption::DELETE_PART},
            {"create", MenuOption::CREATE_WEAPON},
            {"save", MenuOption::SAVE_PARTS},
            {"exit", MenuOption::EXIT}
        };

        auto it = textOptions.find(input);
        if (it != textOptions.end()) {
            return it->second;
        }

        std::cout << "Invalid option. Please enter 1-7 or one of these commands:\n"
            << "search, list, add, delete, create, save, exit\n"
            << "Choose an option: ";
    }
}

void handleSearchPart() {
    std::cout << "Enter part name to search: ";
    std::string name;
    std::getline(std::cin, name);
    name = toLower(trim(name));

    auto searchIn = [&name](const std::vector<Part>& parts) {
        for (const Part& p : parts) {
            if (toLower(p.name).find(name) != std::string::npos) {
                std::cout << "- " << p.name << " (" <<
                    (p.type == PartType::RECEIVER ? "Receiver" :
                        p.type == PartType::BARREL ? "Barrel" : "Sight") << ")\n";
            }
        }
        };

    std::cout << "\n=== SEARCH RESULTS ===\n";
    searchIn(partDB.getParts(PartType::RECEIVER));
    searchIn(partDB.getParts(PartType::BARREL));
    searchIn(partDB.getParts(PartType::SIGHT));
}

void handleListParts() {
    auto listParts = [](const std::vector<Part>& parts, const std::string& type) {
        std::cout << "\n=== " << type << " ===\n";
        for (const Part& p : parts) {
            std::cout << "- " << p.name << "\n";
        }
        };

    listParts(partDB.getParts(PartType::RECEIVER), "RECEIVERS");
    listParts(partDB.getParts(PartType::BARREL), "BARRELS");
    listParts(partDB.getParts(PartType::SIGHT), "SIGHTS");
}

void handleAddPart() {
    Part newPart{};
    std::cout << "Enter part name: ";
    std::getline(std::cin, newPart.name);

    std::cout << "Enter part type (1-Receiver, 2-Barrel, 3-Sight): ";
    int typeChoice;
    std::cin >> typeChoice;
    std::cin.ignore();

    if (typeChoice < 1 || typeChoice > 3) {
        std::cout << "Invalid type selection.\n";
        return;
    }
    newPart.type = static_cast<PartType>(typeChoice - 1);

    auto getNumber = [](const std::string& prompt) -> int {
        std::cout << prompt;
        int num;
        while (!(std::cin >> num)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number: ";
        }
        std::cin.ignore();
        return num;
        };

    newPart.hitDieMod = getNumber("Enter hit die modifier: ");
    newPart.dmgDieMod = getNumber("Enter damage die modifier: ");
    newPart.rangeMinMod = getNumber("Enter range minimum modifier: ");
    newPart.rangeMaxMod = getNumber("Enter range maximum modifier: ");

    std::cout << "Enter weight modifier: ";
    std::cin >> newPart.weightMod;
    std::cout << "Enter value modifier: ";
    std::cin >> newPart.valueMod;
    std::cin.ignore();

    std::cout << "Enter category (1-Ballistic, 2-Plasma, 3-Laser, 4-Heavy, 5-All): ";
    int catChoice;
    std::cin >> catChoice;
    std::cin.ignore();

    switch (catChoice) {
    case 1: newPart.compatibilityMask = CATEGORY_BALLISTIC; break;
    case 2: newPart.compatibilityMask = CATEGORY_PLASMA; break;
    case 3: newPart.compatibilityMask = CATEGORY_LASER; break;
    case 4: newPart.compatibilityMask = CATEGORY_HEAVY; break;
    case 5: newPart.compatibilityMask = CATEGORY_ALL; break;
    default: newPart.compatibilityMask = CATEGORY_ALL;
    }

    partDB.addPart(newPart);
    std::cout << "Part added successfully!\n";
}

void handleDeletePart() {
    std::cout << "Enter exact part name to delete: ";
    std::string name;
    std::getline(std::cin, name);

    if (partDB.removePart(name)) {
        std::cout << "Part deleted successfully.\n";
    }
    else {
        std::cout << "Part not found.\n";
    }
}

void handleSaveParts() {
    std::cout << "\n=== SAVING PARTS ===\n";
    size_t total = partDB.getParts(PartType::RECEIVER).size() +
        partDB.getParts(PartType::BARREL).size() +
        partDB.getParts(PartType::SIGHT).size();

    try {
        partDB.saveToBinary("parts.bin");
        std::cout << "Successfully saved " << total << " parts:\n";
        std::cout << "- Receivers: " << partDB.getParts(PartType::RECEIVER).size() << "\n";
        std::cout << "- Barrels: " << partDB.getParts(PartType::BARREL).size() << "\n";
        std::cout << "- Sights: " << partDB.getParts(PartType::SIGHT).size() << "\n";
    }
    catch (...) {
        std::cerr << "Failed to save parts!\n";
    }
}

void handleExit() {
    std::cout << "\nSaving parts before exiting...\n";
    handleSaveParts();
    std::cout << "Goodbye!\n";
}

void handleMenuOption(MenuOption option) {
    switch (option) {
    case MenuOption::SEARCH_PART: handleSearchPart(); break;
    case MenuOption::LIST_PARTS: handleListParts(); break;
    case MenuOption::ADD_PART: handleAddPart(); break;
    case MenuOption::DELETE_PART: handleDeletePart(); break; case MenuOption::CREATE_WEAPON: {
        std::cout << "Enter weapon name: ";
        std::string weaponName;
        std::getline(std::cin, weaponName);

        Weapon* newWeapon = handleCreateWeapon(weaponName, partDB);
        if (newWeapon) {
            std::cout << "Successfully created weapon: " << newWeapon->name << "\n";
            if (promptSaveWeapon(*newWeapon)) {
                std::cout << "Weapon saved!\n";
            }
            delete newWeapon;  // Clean up
        }
        else {
            std::cout << "Failed to create weapon (missing parts?)\n";
        }
        break;
    }
    case MenuOption::SAVE_PARTS: handleSaveParts(); break;
    case MenuOption::EXIT: handleExit(); break;
    }
}