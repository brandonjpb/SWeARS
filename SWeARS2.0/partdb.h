#pragma once
#include "types.h"
#include <vector>
#include <string>

struct PartDatabase {
    std::vector<Part> receivers;
    std::vector<Part> barrels;
    std::vector<Part> sights;

    void addPart(const Part& part);
    bool removePart(const std::string& name);
    const std::vector<Part>& getParts(PartType type) const;
    void saveToBinary(const std::string& filename) const;
    void loadFromBinary(const std::string& filename);
};

extern PartDatabase partDB;