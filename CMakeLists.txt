cmake_minimum_required(VERSION 3.31)
project(afop_translator_backend VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)
add_subdirectory(src)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")