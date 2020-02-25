cmake_minimum_required(VERSION 3.10)

# set the project name
project(COMP220-Code-Examples)

# find SDL2
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# add the executable
add_executable(COMP220-Code-Examples ${PROJECT_SOURCE_DIR}/src/main.cpp)
target_link_libraries(COMP220-Code-Examples ${SDL2_LIBRARIES})