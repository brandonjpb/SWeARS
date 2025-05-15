#include "gtest/gtest.h"
#include "save.h"
#include <fstream>  // For file operations
#include <cstdio>   // For std::remove

// Test 1: Verify default parts can be saved and loaded
TEST(SaveLoad, RoundtripWithDefaultParts) {
    PartDatabase originalDB;
    initializeDefaultParts(originalDB); // Load defaults

    const std::string testFile = "test_default_parts.bin";

    // Save and load
    ASSERT_TRUE(savePartsToBinary(originalDB, testFile));

    PartDatabase loadedDB;
    ASSERT_TRUE(loadPartsFromBinary(loadedDB, testFile));

    // Verify basics
    EXPECT_EQ(originalDB.receivers.size(), loadedDB.receivers.size());
    EXPECT_EQ(originalDB.barrels[0].name, loadedDB.barrels[0].name);

    // Clean up
    std::remove(testFile.c_str());
}

// Test 2: Loading non-existent file should fail
TEST(SaveLoad, MissingFileFails) {
    PartDatabase db;
    EXPECT_FALSE(loadPartsFromBinary(db, "ghost_file_that_doesnt_exist.bin"));
}

// Test 3: Custom parts should survive save/load
TEST(SaveLoad, SavesCustomParts) {
    PartDatabase db;
    db.addPart({ "Custom Scope", PartType::SIGHT }); // Add custom part

    const std::string testFile = "custom_parts.bin";
    ASSERT_TRUE(savePartsToBinary(db, testFile));

    // Verify load
    PartDatabase loadedDB;
    ASSERT_TRUE(loadPartsFromBinary(loadedDB, testFile));

    bool foundCustom = false;
    for (const auto& part : loadedDB.sights) {
        if (part.name == "Custom Scope") {
            foundCustom = true;
            break;
        }
    }
    EXPECT_TRUE(foundCustom);

    std::remove(testFile.c_str());
}

// Test 4: Initialize should reset to defaults
TEST(SaveLoad, InitializeResetsToDefaults) {
    PartDatabase db;
    db.addPart({ "Temporary Part", PartType::RECEIVER }); // Add dummy

    initializeDefaultParts(db); // Reset

    // Should have exactly 4 default receivers
    EXPECT_EQ(db.receivers.size(), 4);
    EXPECT_EQ(db.receivers[0].name, "Ballistic Receiver");
}

// Test 5: Corrupted files should fail safely
TEST(SaveLoad, CorruptedFileFails) {
    const std::string badFile = "corrupted.bin";

    // Create junk file
    {
        std::ofstream out(badFile, std::ios::binary);
        out << "THIS_IS_NOT_VALID_BINARY_DATA";
    }

    PartDatabase db;
    EXPECT_FALSE(loadPartsFromBinary(db, badFile));
    EXPECT_TRUE(db.receivers.empty()); // Should be reset

    std::remove(badFile.c_str());
}