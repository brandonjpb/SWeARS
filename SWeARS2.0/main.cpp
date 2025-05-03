#include "menu.h"
#include "save.h"
#include <iostream>

int main() {
    std::cout << "=== WEAPON CREATOR ===\n";

    if (!loadPartsFromBinary(partDB)) {
        std::cout << "Loading default parts...\n";
        initializeDefaultParts(partDB);
    }

    while (true) {
        displayMenu();
        MenuOption choice = getMenuChoice();
        if (choice == MenuOption::EXIT) {
            if (!savePartsToBinary(partDB)) {
                std::cerr << "Warning: Failed to save parts!\n";
            }
            break;
        }
        handleMenuOption(choice);
    }

    return 0;
}