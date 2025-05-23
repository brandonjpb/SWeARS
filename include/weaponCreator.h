#pragma once
#include "types.h"
#include "partdb.h"
#include <memory>

class WeaponBuilder {
public:
    explicit WeaponBuilder(const std::string& name);
    void addPart(const Part& part);
    void setNote(const std::string& note);
    std::unique_ptr<Weapon> build();

private:
    std::unique_ptr<Weapon> weapon;
};

// Refactored creation and save function declarations
std::unique_ptr<Weapon> handleCreateWeapon(const std::string& name, PartDatabase& db);
bool promptSaveWeapon(const Weapon& weapon);
void initializeDefaultParts(PartDatabase& db);
