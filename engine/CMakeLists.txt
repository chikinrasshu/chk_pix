cmake_minimum_required(VERSION 3.23)
project(chk_engine LANGUAGES C CXX)

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

if (NOT TARGET chk_win)
    message(FATAL_ERROR "Please include chk_win")
endif ()

if (NOT TARGET chk_soft)
    message(FATAL_ERROR "Please include chk_soft")
endif ()

# Sources and Headers
set(chk_engine_src_raw "engine.cpp")
set(chk_engine_hdr_raw "engine.hpp")

prepend(chk_engine_src "${CMAKE_CURRENT_SOURCE_DIR}/src" ${chk_engine_src_raw})
prepend(chk_engine_hdr "${CMAKE_CURRENT_SOURCE_DIR}/include/engine" ${chk_engine_hdr_raw})

# Executable/Library
add_executable(chk_engine)
target_sources(chk_engine PUBLIC ${chk_engine_src} ${chk_engine_hdr})

target_link_libraries(chk_engine PUBLIC chk_core)
target_link_libraries(chk_engine PUBLIC chk_win)
target_link_libraries(chk_engine PUBLIC chk_soft)

target_include_directories(chk_engine PUBLIC "include")
target_compile_features(chk_engine PUBLIC cxx_std_20)

