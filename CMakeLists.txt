cmake_minimum_required(VERSION 3.25)

# 1. AUTO-LINK THE GEODE BUILDER
find_package(Geode REQUIRED)

# 2. MATCH YOUR ID FROM MOD.JSON
project(zvext.vox_menu VERSION 1.0.0 LANGUAGES CXX)

# 3. CONVERT THE C++ INTO THE MOD FILE
add_geode_mod(zvext.vox_menu main.cpp)
