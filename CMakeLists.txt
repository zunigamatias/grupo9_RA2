cmake_minimum_required(VERSION 3.10)
project(CacheAlgorithms)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(
    algorithms
    algorithms/lfu
    algorithms/random_replacement
    algorithms/s3_fifo
    core
    core/disk_io
    simulation
    docs
)

# Gather all source files
set(SOURCES
    ra2_main.cpp
    algorithms/cache.cpp
    algorithms/lfu/lfu.cpp
    algorithms/random_replacement/randomReplacement.cpp
    algorithms/s3_fifo/s3Fifo.cpp
    core/core.cpp
    core/disk_io/diskIo.cpp
    simulation/simulator.cpp
    docs/docs.cpp
)

# Create the executable
add_executable(cache_simulator ${SOURCES})
