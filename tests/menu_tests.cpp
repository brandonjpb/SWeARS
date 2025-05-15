#include "gtest/gtest.h"
#include "menu.h"
#include <sstream>

// Test 1: Does toLower() make things lowercase?
TEST(MenuHelpers, ToLowerWorks) {
    EXPECT_EQ(toLower("HELLO"), "hello");
    EXPECT_EQ(toLower("MixED"), "mixed");
    EXPECT_EQ(toLower("123"), "123");
    EXPECT_EQ(toLower("MixED123"), "mixed123");
}

// Test 2: Does trim() remove spaces?
TEST(MenuHelpers, TrimWorks) {
    EXPECT_EQ(trim("  wow  "), "wow");
    EXPECT_EQ(trim("no_spaces"), "no_spaces");
    EXPECT_EQ(trim("  "), "");
}

// Test 3: Can we fake user input for menu choices?
TEST(MenuInput, NumberInputsWork) {
    // Simulate "1" text input
    std::istringstream fake_input("1\n");
    std::cin.rdbuf(fake_input.rdbuf());

    EXPECT_EQ(getMenuChoice(), MenuOption::SEARCH_PART);
}

// Test 4: Do text commands work?
TEST(MenuInput, TextInputsWork) {
    // Simulate "exit" text entry
    std::istringstream fake_input("exit\n");
    std::cin.rdbuf(fake_input.rdbuf());

    EXPECT_EQ(getMenuChoice(), MenuOption::EXIT);
}

// Test 5: Does invalid input get rejected?
TEST(MenuInput, BadInputFails) {
    std::istringstream fake_input("99\n1\n");
    std::cin.rdbuf(fake_input.rdbuf());

    testing::internal::CaptureStdout();
    MenuOption result = getMenuChoice();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(result, MenuOption::SEARCH_PART);
    EXPECT_TRUE(output.find("Invalid") != std::string::npos);
}