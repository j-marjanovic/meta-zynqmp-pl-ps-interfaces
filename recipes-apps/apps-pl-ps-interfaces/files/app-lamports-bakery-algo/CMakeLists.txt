cmake_minimum_required(VERSION 3.10)

project(app-lamports-bakery-algo)

add_executable(app-lamports-bakery-algo)
target_sources(app-lamports-bakery-algo PRIVATE src/app-lamports-bakery-algo.cpp)
target_compile_features(app-lamports-bakery-algo PRIVATE cxx_std_17)
target_include_directories(app-lamports-bakery-algo PRIVATE ../common)
target_compile_options(app-lamports-bakery-algo PRIVATE -Wall -Wextra -pedantic -O2)

