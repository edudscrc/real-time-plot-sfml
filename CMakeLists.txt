cmake_minimum_required(VERSION 3.28)
project(CMakeSFMLProject LANGUAGES CXX)

# Set build type to Release by default if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Define optimization flags for different build types
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g -DNDEBUG")
set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os -DNDEBUG")

# Enable Link Time Optimization for Release builds
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Output the current configuration
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ flags: ${CMAKE_CXX_FLAGS}")
message(STATUS "C++ Release flags: ${CMAKE_CXX_FLAGS_RELEASE}")

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.1
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

find_package(Threads REQUIRED)

add_executable(main src/main.cpp src/Grid2D.cpp src/GridManager.cpp src/Point.cpp src/Plot.cpp)
target_compile_features(main PRIVATE cxx_std_17)

# Add additional optimization flags specific to this target if needed
target_compile_options(main PRIVATE 
    $<$<CONFIG:Release>:-march=native>)

target_link_libraries(main PRIVATE SFML::Graphics Threads::Threads)