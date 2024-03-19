cmake_minimum_required(VERSION 3.23)
project(chk_gfx LANGUAGES C CXX)

# Common
function(prepend var prefix)
    set(listVar "")
    foreach (f ${ARGN})
        LIST(APPEND listVar "${prefix}/${f}")
    endforeach (f)
    set(${var} "${listVar}" PARENT_SCOPE)
endfunction(prepend)

# Dependencies
if (NOT TARGET chk_core)
    message(FATAL_ERROR "Please include chk_core")
endif ()

if (NOT TARGET glad)
    message(FATAL_ERROR "Please include glad")
endif ()

# Sources and Headers
set(chk_gfx_src_raw "gfx.cpp" "shader.cpp")
set(chk_gfx_hdr_raw "gfx.hpp" "shader.hpp")

prepend(chk_gfx_src "${CMAKE_CURRENT_SOURCE_DIR}/src" ${chk_gfx_src_raw})
prepend(chk_gfx_hdr "${CMAKE_CURRENT_SOURCE_DIR}/include/gfx" ${chk_gfx_hdr_raw})

# Executable/Library
add_library(chk_gfx)
target_sources(chk_gfx PUBLIC ${chk_gfx_src} ${chk_gfx_hdr})

target_link_libraries(chk_gfx PUBLIC chk_core)
target_link_libraries(chk_gfx PUBLIC glad)

target_include_directories(chk_gfx PUBLIC "include")
target_compile_features(chk_gfx PUBLIC cxx_std_20)

