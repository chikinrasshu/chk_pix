cmake_minimum_required(VERSION 3.27)
project(chk_pix LANGUAGES C CXX)

# External Dependencies
set(GLFW_SOURCES_DIR "${PROJECT_SOURCE_DIR}/ext/glfw/")
add_subdirectory("${GLFW_SOURCES_DIR}")

set(GLM_SOURCES_DIR "${PROJECT_SOURCE_DIR}/ext/glm/")
add_subdirectory("${GLM_SOURCES_DIR}")

set(GLAD_SOURCES_DIR "${PROJECT_SOURCE_DIR}/ext/glad/")
add_subdirectory("${GLAD_SOURCES_DIR}/cmake" glad_cmake)
glad_add_library(glad REPRODUCIBLE API gl:core=4.1)

# Systems
add_subdirectory("sys/core")
add_subdirectory("sys/win")
add_subdirectory("sys/soft")

# Sample Programs
add_subdirectory("engine")
