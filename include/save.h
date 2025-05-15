#pragma once
#include "partdb.h"
#include "weaponCreator.h"
#include <string>
#include <vector>

bool savePartsToBinary(const PartDatabase& db, const std::string& filename = "parts.bin");
bool loadPartsFromBinary(PartDatabase& db, const std::string& filename = "parts.bin");