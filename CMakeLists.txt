cmake_minimum_required(VERSION 3.13)

project(SpringAI LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS
    src/*.cpp
)

find_package(cpr CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_executable(springai ${SOURCE_FILES})

install(TARGETS springai
    RUNTIME DESTINATION bin
)

target_include_directories(springai PRIVATE
    include
)

target_link_libraries(springai PRIVATE
    cpr::cpr
    nlohmann_json::nlohmann_json
)