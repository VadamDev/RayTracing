cmake_minimum_required(VERSION 3.31)
project(Raytracing)

set(CMAKE_CXX_STANDARD 20)

# Dependencies
find_package(spdlog REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(tinyobjloader CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

file(GLOB_RECURSE PROJECT_SOURCE_FILES src/*.cpp)

# Source files
add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_FILES})

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        spdlog::spdlog
        glad::glad
        glfw
        glm::glm
        nlohmann_json::nlohmann_json
        tinyobjloader::tinyobjloader
        imgui::imgui
)

# portable-file-dialogs
find_path(PFD_INCLUDE_DIR "portable-file-dialogs.h")
if(PFD_INCLUDE_DIR)
    target_include_directories(${PROJECT_NAME} PRIVATE ${PFD_INCLUDE_DIR})
else()
    message(FATAL_ERROR "Could not find portable-file-dialogs.h in vcpkg")
endif()

# Copy Assets
set(ASSET_OUT_DIR ${CMAKE_BINARY_DIR}/resources)

add_custom_target(copy_assets ALL
        COMMAND ${CMAKE_COMMAND} -E make_directory ${ASSET_OUT_DIR}
        COMMAND ${CMAKE_COMMAND} -E rm -rf ${ASSET_OUT_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/resources ${ASSET_OUT_DIR}
)

add_dependencies(${PROJECT_NAME} copy_assets)
