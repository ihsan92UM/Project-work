cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(sts40+lis2dtw12_v1)

# Include the "include" directory for header files
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Include source files
target_sources(app PRIVATE src/main.c 
                           src/sts40.c 
                           src/lis2dtw12.c)