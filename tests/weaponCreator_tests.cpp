#include "gtest/gtest.h"
#include "weaponCreator.h"
#include "partdb.h"
#include <fstream>
#include <cstdio>

// Test helper function
TEST(WeaponCreatorHelpers, SanitizeWeaponName) {
    EXPECT_EQ(sanitizeWeaponNameToFilename("Plasma Rifle"), "plasma_rifle.txt");
    EXPECT_EQ(sanitizeWeaponNameToFilename("Heavy!@#Gun"), "heavygun.txt");
    EXPECT_EQ(sanitizeWeaponNameToFilename(""), "weapon.txt");
}

// Test weapon creation basics
TEST(WeaponCreation, CreatesValidWeapon) {
    // Redirect std::cin for testing
    std::stringstream inBuffer;
    std::streambuf* oldCin = std::cin.rdbuf();
    std::streambuf* oldCout = std::cout.rdbuf();
    std::stringstream outBuffer;

    // Swap the buffers
    std::cin.rdbuf(inBuffer.rdbuf());
    std::cout.rdbuf(outBuffer.rdbuf());

    // Set up test input - select first option for each choice
    inBuffer << "1\n1\n1\nTest note\n";

    PartDatabase db;
    initializeDefaultParts(db);  // Ensure defaults exist

    Weapon* weapon = handleCreateWeapon("Test Weapon", db);

    // Restore original streams before assertions
    std::cin.rdbuf(oldCin);
    std::cout.rdbuf(oldCout);

    ASSERT_NE(weapon, nullptr);

    // Verify basic stats exist
    EXPECT_GT(weapon->damageQuantity, 0);
    EXPECT_GE(weapon->rangeMax, weapon->rangeMin);

    delete weapon;  // Clean up
}

// Test weapon saving
TEST(WeaponSaving, SavesToFile) {
    // Redirect cin for testing
    std::stringstream inBuffer;
    std::streambuf* oldCin = std::cin.rdbuf();
    std::cin.rdbuf(inBuffer.rdbuf());

    // Setup mock input
    inBuffer << "y" << std::endl;

    Weapon testWeapon;
    testWeapon.name = "Test_Save";
    testWeapon.damageQuantity = 2;
    testWeapon.damageQuality = 4;
    testWeapon.rangeMin = 10;
    testWeapon.rangeMax = 50;
    testWeapon.weight = 5;
    testWeapon.value = 100;
    testWeapon.noteOrFlavor = "Test note";

    const std::string filename = sanitizeWeaponNameToFilename(testWeapon.name);

    // Test saving
    bool saved = promptSaveWeapon(testWeapon);

    // Restore cin before assertions
    std::cin.rdbuf(oldCin);

    EXPECT_TRUE(saved);

    // Verify file contents
    std::ifstream in(filename);
    EXPECT_TRUE(in.good());

    std::string line;
    std::getline(in, line);
    EXPECT_TRUE(line.find("Test_Save") != std::string::npos);

    // Clean up
    in.close();
    std::remove(filename.c_str());
}

// Test invalid weapon handling
TEST(WeaponCreation, HandlesEmptyDatabase) {
    // Redirect stdout to avoid cluttering test output
    std::streambuf* oldCout = std::cout.rdbuf();
    std::stringstream outBuffer;
    std::cout.rdbuf(outBuffer.rdbuf());

    PartDatabase emptyDB;  // No parts added
    Weapon* weapon = handleCreateWeapon("Should Fail", emptyDB);

    // Restore stdout
    std::cout.rdbuf(oldCout);

    EXPECT_EQ(weapon, nullptr);  // Should fail gracefully
}