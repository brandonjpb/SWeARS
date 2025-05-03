#include "partdb.h"
#include <fstream>
#include <algorithm>

std::map<std::string, PartType> partTypeMap = {
    {"receiver", PartType::RECEIVER},
    {"barrel", PartType::BARREL},
    {"sight", PartType::SIGHT}
};

std::map<std::string, Category> categoryMap = {
    {"ballistic", CATEGORY_BALLISTIC},
    {"plasma", CATEGORY_PLASMA},
    {"laser", CATEGORY_LASER},
    {"heavy", CATEGORY_HEAVY},
    {"all", CATEGORY_ALL},
    {"none", CATEGORY_NONE}
};

PartDatabase partDB{
    // Default receivers
    {
        {"Ballistic Receiver", PartType::RECEIVER, 1, 2, 60, 180, 4, 170, CATEGORY_BALLISTIC, CATEGORY_NONE},
        {"Plasma Receiver", PartType::RECEIVER, 2, 2, 60, 120, 7, 225, CATEGORY_PLASMA, CATEGORY_NONE},
        {"Laser Receiver", PartType::RECEIVER, 3, 1, 20, 180, 5, 420, CATEGORY_LASER, CATEGORY_NONE},
        {"Heavy Receiver", PartType::RECEIVER, 4, 2, 60, 120, 14, 600, CATEGORY_HEAVY, CATEGORY_NONE}
    },
    // Default barrels
    {
        {"Short Barrel", PartType::BARREL, 0, 0, -20, -60, -0.5, 0, CATEGORY_ALL, CATEGORY_NONE},
        {"Standard Barrel", PartType::BARREL, 0, 0, 0, 0, 0, 0, CATEGORY_ALL, CATEGORY_NONE},
        {"Long Barrel", PartType::BARREL, 0, 0, 20, 60, 1.2, 25, CATEGORY_ALL, CATEGORY_NONE}
    },
    // Default sights
    {
        {"Iron Sight", PartType::SIGHT, 0, 0, 0, 0, 0, 0, CATEGORY_ALL, CATEGORY_NONE},
        {"4x Scope", PartType::SIGHT, 0, 0, 120, 240, 2, 130, CATEGORY_ALL, CATEGORY_HEAVY}
    }
};

void PartDatabase::addPart(const Part& part) {
    switch (part.type) {
    case PartType::RECEIVER: receivers.push_back(part); break;
    case PartType::BARREL: barrels.push_back(part); break;
    case PartType::SIGHT: sights.push_back(part); break;
    }
}

bool PartDatabase::removePart(const std::string& name) {
    auto removeFrom = [&](std::vector<Part>& parts) {
        auto it = std::remove_if(parts.begin(), parts.end(),
            [&](const Part& p) { return p.name == name; });
        bool found = it != parts.end();
        parts.erase(it, parts.end());
        return found;
        };

    return removeFrom(receivers) || removeFrom(barrels) || removeFrom(sights);
}

const std::vector<Part>& PartDatabase::getParts(PartType type) const {
    switch (type) {
    case PartType::RECEIVER: return receivers;
    case PartType::BARREL: return barrels;
    case PartType::SIGHT: return sights;
    default: throw std::invalid_argument("Invalid part type");
    }
}

void PartDatabase::saveToBinary(const std::string& filename) const {
    std::ofstream out(filename, std::ios::binary);
    if (!out) throw std::runtime_error("Failed to open file for writing");

    auto writeVector = [&](const std::vector<Part>& parts) {
        size_t count = parts.size();
        out.write(reinterpret_cast<const char*>(&count), sizeof(count));
        for (const Part& p : parts) {
            size_t nameLen = p.name.size();
            out.write(reinterpret_cast<const char*>(&nameLen), sizeof(nameLen));
            out.write(p.name.c_str(), nameLen);
            out.write(reinterpret_cast<const char*>(&p), offsetof(Part, name) + nameLen);
        }
        };

    writeVector(receivers);
    writeVector(barrels);
    writeVector(sights);
}

void PartDatabase::loadFromBinary(const std::string& filename) {
    std::ifstream in(filename, std::ios::binary);
    if (!in) {
        throw std::runtime_error("Could not open file");
    }

    auto readVector = [&](std::vector<Part>& parts) {
        size_t count;
        in.read(reinterpret_cast<char*>(&count), sizeof(count));
        if (!in) throw std::runtime_error("Error reading count");

        parts.resize(count);
        for (Part& p : parts) {
            size_t nameLen;
            in.read(reinterpret_cast<char*>(&nameLen), sizeof(nameLen));
            if (!in) throw std::runtime_error("Error reading name length");

            p.name.resize(nameLen);
            in.read(&p.name[0], nameLen);
            in.read(reinterpret_cast<char*>(&p) + offsetof(Part, name), sizeof(Part) - offsetof(Part, name));
            if (!in) throw std::runtime_error("Error reading part data");
        }
        };

    readVector(receivers);
    readVector(barrels);
    readVector(sights);
}