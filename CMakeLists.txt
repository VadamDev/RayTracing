cmake_minimum_required(VERSION 3.31)
project(GravitySim)

set(CMAKE_CXX_STANDARD 20)

find_package(glfw3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(GravitySim ../GravitySim/src/main.cpp
        ../GravitySim/src/engine/window/Window.cpp
        ../GravitySim/src/engine/window/Window.h
)

target_link_libraries(${PROJECT_NAME}
        glfw
        OpenGL::GL
)
