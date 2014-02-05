cmake_minimum_required(VERSION 2.8)
project(MRF-DD)


file (GLOB MRF-DD_SOURCES
    "*.cpp"
    "*.h"
)

add_executable(MRF-DD ${MRF-DD_SOURCES})
