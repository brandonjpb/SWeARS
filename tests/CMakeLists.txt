# tests/CMakeLists.txt
include(FetchContent)
FetchContent_Declare(
    googletest    
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

add_executable(SWeARS_Tests
    menu_tests.cpp
    partdb_tests.cpp
    save_tests.cpp
    weaponCreator_tests.cpp
)

target_link_libraries(SWeARS_Tests PRIVATE
    SWeARS_Lib
    gtest_main
)

enable_testing()
add_test(NAME AllTests COMMAND SWeARS_Tests)