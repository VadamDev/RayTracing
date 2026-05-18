cmake_minimum_required(VERSION 3.31)
project(Raytracing)

set(CMAKE_CXX_STANDARD 20)

# Hack, to prevent Windows API from bitching
if(MSVC)
    add_compile_definitions(NOMINMAX)
endif()

# DEPENDENCIES
find_package(spdlog REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(tinyobjloader CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(imguizmo CONFIG REQUIRED)
find_package(implot CONFIG REQUIRED)

# portable-file-dialogs doesn't have a CMake config file...
find_path(PFD_INCLUDE_DIR "portable-file-dialogs.h" REQUIRED)
add_library(portable-file-dialogs INTERFACE)
target_include_directories(portable-file-dialogs INTERFACE ${PFD_INCLUDE_DIR})

# SOURCE FILES
file(GLOB_RECURSE PROJECT_SOURCE_FILES src/*.cpp)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_FILES})

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        spdlog::spdlog
        glad::glad
        glfw
        glm::glm
        EnTT::EnTT
        nlohmann_json::nlohmann_json
        tinyobjloader::tinyobjloader
        imgui::imgui
        imguizmo::imguizmo
        implot::implot
        portable-file-dialogs
)

# ImViewGuizmo is not available with VCPKG so:
target_include_directories(${PROJECT_NAME} PRIVATE thirdparty/ImViewGuizmo)

# Copy Assets TODO: FIXME! this is ugly
set(ASSET_OUT_DIR ${CMAKE_BINARY_DIR}/resources)

add_custom_target(copy_assets ALL
        COMMAND ${CMAKE_COMMAND} -E make_directory ${ASSET_OUT_DIR}
        COMMAND ${CMAKE_COMMAND} -E rm -rf ${ASSET_OUT_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/resources ${ASSET_OUT_DIR}
)

add_dependencies(${PROJECT_NAME} copy_assets)
