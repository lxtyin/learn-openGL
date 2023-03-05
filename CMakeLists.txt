cmake_minimum_required(VERSION 3.21)
project(openGL)

file(GLOB SRC_FILES
        ${PROJECT_SOURCE_DIR}/main.cpp
        ${PROJECT_SOURCE_DIR}/include/*.hpp
        ${PROJECT_SOURCE_DIR}/include/glad/*.c
        ${PROJECT_SOURCE_DIR}/src/*.h
        ${PROJECT_SOURCE_DIR}/src/*.cpp
        ${PROJECT_SOURCE_DIR}/src/tool/*.cpp
        ${PROJECT_SOURCE_DIR}/src/tool/*.hpp
        ${PROJECT_SOURCE_DIR}/src/material/*.h
        ${PROJECT_SOURCE_DIR}/src/material/*.cpp
        )

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
link_directories(lib)
add_executable(openGL ${SRC_FILES} src/material/PhongMaterial.cpp src/material/PhongMaterial.h src/material/DisneyMaterial.cpp src/material/DisneyMaterial.h)
target_link_libraries(openGL opengl32 glu32 libglfw3.a libzlibstatic.a libassimp.dll.a)
