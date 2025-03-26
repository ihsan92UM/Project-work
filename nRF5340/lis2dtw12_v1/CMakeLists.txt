cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(lis2dtw12_v1)

# Add application source files
target_sources(app PRIVATE src/main.c src/lis2dtw12.c)

