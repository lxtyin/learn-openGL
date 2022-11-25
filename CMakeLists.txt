cmake_minimum_required(VERSION 3.21)
project(openGL)

file(GLOB SRC_FILES
        ${PROJECT_SOURCE_DIR}/main.cpp
        ${PROJECT_SOURCE_DIR}/include/*.h
        ${PROJECT_SOURCE_DIR}/include/*.hpp
#        ${PROJECT_SOURCE_DIR}/include/assimp/*.h
#        ${PROJECT_SOURCE_DIR}/include/assimp/*.hpp
#        ${PROJECT_SOURCE_DIR}/include/glfw/*.h
#        ${PROJECT_SOURCE_DIR}/include/glad/*.h
#        ${PROJECT_SOURCE_DIR}/include/glm/*.h
#        ${PROJECT_SOURCE_DIR}/include/glm/*.hpp
        ${PROJECT_SOURCE_DIR}/src/glad/*.c
        ${PROJECT_SOURCE_DIR}/src/glad/*.cpp
        ${PROJECT_SOURCE_DIR}/src/tool/*.c
        ${PROJECT_SOURCE_DIR}/src/tool/*.cpp
        ${PROJECT_SOURCE_DIR}/src/*.c
        ${PROJECT_SOURCE_DIR}/src/*.cpp)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
link_directories(lib)
add_executable(openGL ${SRC_FILES})
target_link_libraries(openGL opengl32 glu32 libglfw3.a libzlibstatic.a libassimp.dll.a)
