cmake_minimum_required(VERSION 3.10)

project(app-axi-traffic-gen)

find_package(Boost 1.74 COMPONENTS program_options REQUIRED)

add_executable(app-axi-traffic-gen)
target_sources(app-axi-traffic-gen PRIVATE src/app-axi-traffic-gen.cpp)
target_compile_features(app-axi-traffic-gen PRIVATE cxx_std_20)
target_include_directories(app-axi-traffic-gen PRIVATE ../common)
target_compile_options(app-axi-traffic-gen PRIVATE -Wall -Wextra -pedantic -O2)
target_link_libraries(app-axi-traffic-gen PRIVATE Boost::program_options)
