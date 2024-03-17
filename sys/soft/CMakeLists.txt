cmake_minimum_required(VERSION 3.23)
project(chk_soft LANGUAGES C CXX)

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

# Sources and Headers
set(chk_soft_src_raw "soft.cpp")
set(chk_soft_hdr_raw "soft.hpp")

prepend(chk_soft_src "${CMAKE_CURRENT_SOURCE_DIR}/src" ${chk_soft_src_raw})
prepend(chk_soft_hdr "${CMAKE_CURRENT_SOURCE_DIR}/include/soft" ${chk_soft_hdr_raw})

# Executable/Library
add_library(chk_soft)
target_sources(chk_soft PUBLIC ${chk_soft_src} ${chk_soft_hdr})

target_link_libraries(chk_soft PUBLIC chk_core)

target_include_directories(chk_soft PUBLIC "include")
target_compile_features(chk_soft PUBLIC cxx_std_20)

