cmake_minimum_required(VERSION 3.23)
project(chk_win LANGUAGES C CXX)

# Common
function(prepend var prefix)
    set(listVar "")
    foreach (f ${ARGN})
        LIST(APPEND listVar "${prefix}/${f}")
    endforeach (f)
    set(${var} "${listVar}" PARENT_SCOPE)
endfunction(prepend)

# Dependencies
if (NOT TARGET glfw)
    find_package(glfw3 CONFIG REQUIRED)
endif ()

if (NOT TARGET glad)
    message(FATAL_ERROR "Please include glad2")
endif ()

if (NOT TARGET chk_core)
    message(FATAL_ERROR "Please include chk_core")
endif ()

# Sources and Headers
set(chk_win_src_raw
        "framebuffer.cpp"
        "win.cpp"
)
set(chk_win_hdr_raw
        "framebuffer.hpp"
        "win.hpp"
)

prepend(chk_win_src "${CMAKE_CURRENT_SOURCE_DIR}/src" ${chk_win_src_raw})
prepend(chk_win_hdr "${CMAKE_CURRENT_SOURCE_DIR}/include/win" ${chk_win_hdr_raw})

# Executable/Library
add_library(chk_win)
target_sources(chk_win PUBLIC ${chk_win_src} ${chk_win_hdr})

target_link_libraries(chk_win PUBLIC chk_core)
target_link_libraries(chk_win PUBLIC glfw)
target_link_libraries(chk_win PUBLIC glad)

target_include_directories(chk_win PUBLIC "include")
target_compile_features(chk_win PUBLIC cxx_std_20)
