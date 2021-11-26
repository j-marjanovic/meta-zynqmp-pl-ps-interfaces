cmake_minimum_required(VERSION 3.10)

project(app-axi-proxy)

add_executable(app-axi-proxy)
target_sources(app-axi-proxy PRIVATE src/app-axi-proxy.cpp)
target_compile_features(app-axi-proxy PRIVATE cxx_std_17)
target_include_directories(app-axi-proxy PRIVATE ../common)
target_compile_options(app-axi-proxy PRIVATE -Wall -Wextra -pedantic -O2)
