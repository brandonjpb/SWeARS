#pragma once
#include "types.h"
#include "partdb.h"

Weapon* handleCreateWeapon(const std::string& name, PartDatabase& db);
bool promptSaveWeapon(const Weapon& weapon);
void initializeDefaultParts(PartDatabase& db);