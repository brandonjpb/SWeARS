# CMakeLists.txt
cmake_minimum_required(VERSION 3.15)
project(SWeARS2.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_definitions(_SILENCE_CXX17_ADAPTOR_TYPEDEFS_DEPRECATION_WARNING)
endif()

add_library(SWeARS_Lib STATIC 
    src/menu.cpp
    src/partdb.cpp
    src/save.cpp
    src/weaponCreator.cpp
)
target_include_directories(SWeARS_Lib PUBLIC include)

add_executable(SWeARS2.0 src/main.cpp)
target_link_libraries(SWeARS2.0 PRIVATE SWeARS_Lib)

enable_testing()
add_subdirectory(tests)