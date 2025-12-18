cmake_minimum_required(VERSION 3.31)
project(GravitySim)

set(CMAKE_CXX_STANDARD 20)

find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

add_executable(GravitySim
        src/main.cpp
        src/engine/window/Window.cpp
        src/game/Game.cpp
        src/engine/clock/FixedStepClock.cpp
        src/engine/graphics/shader/ShaderProgram.cpp
)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
            glad::glad
            glfw
            glm::glm
)
