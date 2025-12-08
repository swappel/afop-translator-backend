cmake_minimum_required(VERSION 3.31)
project(afop_translator_backend VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.12.0
)
FetchContent_MakeAvailable(nlohmann_json)

FetchContent_Declare(
        Crow
        GIT_REPOSITORY https://github.com/ipkn/crow.git
        GIT_TAG master
)
FetchContent_MakeAvailable(Crow)

include_directories(include)
add_subdirectory(src)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")