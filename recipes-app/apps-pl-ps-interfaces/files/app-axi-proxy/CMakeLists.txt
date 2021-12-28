cmake_minimum_required(VERSION 3.10)

project(app-axi-proxy)

find_package(Boost 1.74 COMPONENTS program_options REQUIRED)

add_executable(app-axi-proxy)
target_sources(app-axi-proxy PRIVATE src/app-axi-proxy.cpp)
target_compile_features(app-axi-proxy PRIVATE cxx_std_20)
target_include_directories(app-axi-proxy PRIVATE ../common)
target_compile_options(app-axi-proxy PRIVATE -Wall -Wextra -pedantic -O2)
target_link_libraries(app-axi-proxy PRIVATE Boost::program_options)
