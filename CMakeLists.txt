cmake_minimum_required(VERSION 3.25)
find_package(Geode REQUIRED)
project(vox VERSION 1.0.0 LANGUAGES CXX)
add_geode_mod(vox main.cpp)
