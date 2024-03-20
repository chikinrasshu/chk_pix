cmake_minimum_required(VERSION 3.27)
project(chk_pix LANGUAGES C CXX)

# External Dependencies
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_SOURCES_DIR "${PROJECT_SOURCE_DIR}/ext/glfw/")
add_subdirectory("${GLFW_SOURCES_DIR}")

set(GLM_ENABLE_CXX_20 ON CACHE BOOL "" FORCE)
set(GLM_SOURCES_DIR "${PROJECT_SOURCE_DIR}/ext/glm/")
add_subdirectory("${GLM_SOURCES_DIR}")

set(STB_SOURCES_DIR "${PROJECT_SOURCE_DIR}/ext/stb")
add_subdirectory("${STB_SOURCES_DIR}")

set(FMT_SOURCES_DIR "${PROJECT_SOURCE_DIR}/ext/fmt")
add_subdirectory("${FMT_SOURCES_DIR}")

# set(SPDLOG_USE_STD_FORMAT ON CACHE BOOL "" FORCE)
set(SPDLOG_FMT_EXTERNAL ON CACHE BOOL "" FORCE)
set(SPDLOG_SOURCES_DIR "${PROJECT_SOURCE_DIR}/ext/spdlog")
add_subdirectory("${SPDLOG_SOURCES_DIR}")

set(GLAD_SOURCES_DIR "${PROJECT_SOURCE_DIR}/ext/glad/")
add_subdirectory("${GLAD_SOURCES_DIR}/cmake" glad_cmake)
glad_add_library(glad REPRODUCIBLE API gl:core=4.1)


# Systems
add_subdirectory("sys/core")
add_subdirectory("sys/win")
add_subdirectory("sys/gfx")

# Sample Programs
add_subdirectory("engine")
