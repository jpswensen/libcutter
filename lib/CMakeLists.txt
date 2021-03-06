project (libcutter)
cmake_minimum_required(VERSION 2.6.4)
    if(COMMAND cmake_policy)
      cmake_policy(SET CMP0003 NEW)
    endif(COMMAND cmake_policy)
include_directories(${PROJECT_SOURCE_DIR}/include/ ${PROJECT_SOURCE_DIR}/include/pub)

find_package(SWIG)
if(SWIG_FOUND)
    include(${SWIG_USE_FILE})

    find_package(PythonLibs)
    if(PYTHONLIBS_FOUND)
        include_directories(${PYTHON_INCLUDE_PATH})

        set(CMAKE_SWIG_FLAGS "")

        set_source_files_properties(cutter.i PROPERTIES CPLUSPLUS ON)
    endif(PYTHONLIBS_FOUND)
endif(SWIG_FOUND)

find_library(HAS_SDL NAMES SDL SDL_gfx)

set(cutter_files
    serial_port.cpp
    device.cpp
    device_c.cpp
    btea.c
)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    list(REMOVE_AT cutter_files list(FIND serial_port.cpp))
    list(APPEND cutter_files serial_port_win32.cpp)
endif(CMAKE_SYSTEM_NAME STREQUAL "Windows")

if(HAS_SDL)
    list(APPEND cutter_files device_sim.cpp)
endif(HAS_SDL)

add_library (cutter SHARED ${cutter_files})
if(HAS_SDL)
    target_link_libraries (cutter SDL SDL_gfx)
endif(HAS_SDL)

if(PYTHONLIBS_FOUND)
    swig_add_module(cutter python cutter.i ${cutter_files})
    swig_link_libraries(cutter ${PYTHON_LIBRARIES})
    if(HAS_SDL)
        swig_link_libraries(cutter SDL SDL_gfx)
    endif(HAS_SDL)
endif(PYTHONLIBS_FOUND)
