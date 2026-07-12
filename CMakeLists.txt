cmake_minimum_required(VERSION 3.13)

project(codecrafters-claude-code LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS
    src/*.cpp
)

find_package(cpr CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_executable(claude-code ${SOURCE_FILES})

target_include_directories(claude-code PRIVATE
    include
)

target_link_libraries(claude-code PRIVATE
    cpr::cpr
    nlohmann_json::nlohmann_json
)