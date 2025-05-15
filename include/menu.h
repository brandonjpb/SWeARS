#pragma once
#include "types.h"
#include <unordered_map>
#include <string>
#include <map>


enum class MenuOption {
    SEARCH_PART,
    LIST_PARTS,
    ADD_PART,
    DELETE_PART,
    CREATE_WEAPON,
    SAVE_PARTS,
    EXIT
};

void displayMenu();
MenuOption getMenuChoice();
void handleMenuOption(MenuOption option);
void handleSearchPart();
void handleListParts();
void handleAddPart();
void handleDeletePart();
void handleSaveParts();
void handleExit();

std::string toLower(const std::string& str);