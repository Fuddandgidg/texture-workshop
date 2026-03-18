cmake_minimum_required(VERSION 3.21)

# Geode v5.0+ (for GD 2.2081) works best with C++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Standard macOS/iOS architecture setup
if ("${CMAKE_SYSTEM_NAME}" STREQUAL "iOS" OR IOS)
    set(CMAKE_OSX_ARCHITECTURES "arm64")
else()
    set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
endif()

set(CMAKE_CXX_VISIBILITY_PRESET hidden)

# Project name and version (matches your mod.json version)
project(TextureWorkshop VERSION 1.5.0)

# Define your source files clearly
set(SRC_FILES
    src/main.cpp
    src/boobs.cpp
    src/TextureWorkshopLayer.cpp
    src/TexturePack.cpp
    src/TexturePackInfo.cpp
)

# Initialize Geode SDK
if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable.")
endif()

add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)

# Create the mod library
add_library(${PROJECT_NAME} SHARED ${SRC_FILES})

# Setup the .geode package
setup_geode_mod(${PROJECT_NAME})