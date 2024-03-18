cmake_minimum_required(VERSION 3.23)
project(chk_core LANGUAGES C CXX)

# Common
function(prepend var prefix)
    set(listVar "")
    foreach (f ${ARGN})
        LIST(APPEND listVar "${prefix}/${f}")
    endforeach (f)
    set(${var} "${listVar}" PARENT_SCOPE)
endfunction(prepend)

# Dependencies
if (NOT TARGET glm)
    find_package(glm CONFIG REQUIRED)
endif ()

if (NOT TARGET image)
    find_package(stb CONFIG REQUIRED)
endif ()

# Sources and Headers
set(chk_core_src_raw "core.cpp" "bmp.cpp" "file.cpp" "mapping.cpp")
set(chk_core_hdr_raw "core.hpp" "bmp.hpp" "file.hpp" "fn.hpp" "mapping.hpp" "vec.hpp")

prepend(chk_core_src "${CMAKE_CURRENT_SOURCE_DIR}/src" ${chk_core_src_raw})
prepend(chk_core_hdr "${CMAKE_CURRENT_SOURCE_DIR}/include/core" ${chk_core_hdr_raw})

# Executable/Library
add_library(chk_core)
target_sources(chk_core PUBLIC ${chk_core_src} ${chk_core_hdr})

target_link_libraries(chk_core PUBLIC glm)
target_link_libraries(chk_core PUBLIC stb::image)

target_include_directories(chk_core PUBLIC "include")
target_compile_features(chk_core PUBLIC cxx_std_20)

