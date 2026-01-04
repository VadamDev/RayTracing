cmake_minimum_required(VERSION 3.31)
project(GravitySim)

set(CMAKE_CXX_STANDARD 20)

# Dependencies
find_package(spdlog REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

# Source files
add_executable(${PROJECT_NAME}
        src/main.cpp

        src/engine/clock/AbstractClock.h
        src/engine/clock/FixedStepClock.cpp
        src/engine/clock/FixedStepClock.h

        src/engine/graphics/shader/ShaderProgram.cpp
        src/engine/graphics/shader/ShaderProgram.h

        src/engine/utils/FileReader.h

        src/engine/window/Window.cpp
        src/engine/window/Window.h

        src/engine/AbstractGame.h

        src/game/Game.cpp
        src/game/Game.h
        src/engine/GameLifecycle.h
        src/engine/graphics/shader/exceptions/ShaderException.h
        src/game/TestShader.cpp
        src/game/TestShader.h
        src/engine/graphics/shader/UniformAccess.cpp
        src/engine/graphics/shader/UniformAccess.h
        src/engine/graphics/mesh/VertexArrayObject.cpp
        src/engine/graphics/mesh/VertexArrayObject.h
        src/engine/graphics/rendering/Renderable.h
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        spdlog::spdlog
        glad::glad
        glfw
        glm::glm
)

# Copy Assets
set(ASSET_OUT_DIR ${CMAKE_BINARY_DIR}/resources)

add_custom_target(copy_assets ALL
        COMMAND ${CMAKE_COMMAND} -E make_directory ${ASSET_OUT_DIR}
        COMMAND ${CMAKE_COMMAND} -E rm -rf ${ASSET_OUT_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/resources ${ASSET_OUT_DIR}
)

add_dependencies(${PROJECT_NAME} copy_assets)
