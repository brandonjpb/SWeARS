#include "save.h"
#include <fstream>
#include <iostream>
#include <cstring>

// Helper function for safe writing
template<typename T>
void writeBinary(std::ofstream& out, const T& value) {
    out.write(reinterpret_cast<const char*>(&value), sizeof(T));
}

// Helper function for safe reading
template<typename T>
void readBinary(std::ifstream& in, T& value) {
    in.read(reinterpret_cast<char*>(&value), sizeof(T));
}

bool savePartsToBinary(const PartDatabase& db, const std::string& filename) {
    std::ofstream out(filename, std::ios::binary | std::ios::app);
    if (!out) return false;

    try {
        // Save receivers
        writeBinary(out, db.receivers.size());
        for (const auto& part : db.receivers) {
            writeBinary(out, part.name.size());
            out.write(part.name.c_str(), part.name.size());
            out.write(reinterpret_cast<const char*>(&part) + offsetof(Part, type),
                sizeof(Part) - offsetof(Part, type));
        }

        // Save barrels
        writeBinary(out, db.barrels.size());
        for (const auto& part : db.barrels) {
            writeBinary(out, part.name.size());
            out.write(part.name.c_str(), part.name.size());
            out.write(reinterpret_cast<const char*>(&part) + offsetof(Part, type),
                sizeof(Part) - offsetof(Part, type));
        }

        // Save sights
        writeBinary(out, db.sights.size());
        for (const auto& part : db.sights) {
            writeBinary(out, part.name.size());
            out.write(part.name.c_str(), part.name.size());
            out.write(reinterpret_cast<const char*>(&part) + offsetof(Part, type),
                sizeof(Part) - offsetof(Part, type));
        }

        return true;
    }
    catch (...) {
        return false;
    }
}

bool loadPartsFromBinary(PartDatabase& db, const std::string& filename) {
    std::ifstream in(filename, std::ios::binary);
    if (!in) return false;

    try {
        // Clear existing parts
        db = PartDatabase();

        auto loadPartVector = [&](std::vector<Part>& parts) {
            size_t count;
            readBinary(in, count);
            parts.resize(count);

            for (auto& part : parts) {
                size_t nameSize;
                readBinary(in, nameSize);

                part.name.resize(nameSize);
                in.read(&part.name[0], nameSize);

                in.read(reinterpret_cast<char*>(&part) + offsetof(Part, type),
                    sizeof(Part) - offsetof(Part, type));
            }
            };

        loadPartVector(db.receivers);
        loadPartVector(db.barrels);
        loadPartVector(db.sights);

        return true;
    }
    catch (...) {
        db = PartDatabase(); // Reset on failure
        return false;
    }
}

void initializeDefaultParts(PartDatabase& db) {
    // Clear existing parts
    db = PartDatabase();

    // Default Receivers
    db.addPart({ "Ballistic Receiver", PartType::RECEIVER, 1, 2, 60, 180, 4, 170, CATEGORY_BALLISTIC, CATEGORY_NONE });
    db.addPart({ "Plasma Receiver", PartType::RECEIVER, 2, 2, 60, 120, 7, 225, CATEGORY_PLASMA, CATEGORY_NONE });
    db.addPart({ "Laser Receiver", PartType::RECEIVER, 3, 1, 20, 180, 5, 420, CATEGORY_LASER, CATEGORY_NONE });
    db.addPart({ "Heavy Receiver", PartType::RECEIVER, 4, 2, 60, 120, 14, 600, CATEGORY_HEAVY, CATEGORY_NONE });

    // Default Barrels
    db.addPart({ "Short Barrel", PartType::BARREL, 0, 0, -20, -60, -0.5, 0, CATEGORY_ALL, CATEGORY_NONE });
    db.addPart({ "Standard Barrel", PartType::BARREL, 0, 0, 0, 0, 0, 0, CATEGORY_ALL, CATEGORY_NONE });
    db.addPart({ "Long Barrel", PartType::BARREL, 0, 0, 20, 60, 1.2, 25, CATEGORY_ALL, CATEGORY_NONE });

    // Default Sights
    db.addPart({ "Iron Sight", PartType::SIGHT, 0, 0, 0, 0, 0, 0, CATEGORY_ALL, CATEGORY_NONE });
    db.addPart({ "4x Scope", PartType::SIGHT, 0, 0, 120, 240, 2, 130, CATEGORY_ALL, CATEGORY_HEAVY });
}