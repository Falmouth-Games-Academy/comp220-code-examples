cmake_minimum_required(VERSION 3.10)

# set the project name
project(COMP220-Code-Examples)

# find SDL2
find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(GLM REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} {GLEW_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS})



# add the executable
include_directories(${PROJECT_SOURCE_DIR}/src)
add_executable(COMP220-Code-Examples main.cpp model.cpp Texture.cpp Shader.cpp)
target_link_libraries(COMP220-Code-Examples ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${GLEW_LIBRARIES})