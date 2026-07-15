cmake_minimum_required(VERSION 3.20)
project(MiniSurvivors LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/utf-8)
endif()

if(WIN32)
    set(RAYLIB_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../2-Pong/thirdparty/raylib/include")

    if(MSVC AND CMAKE_SIZEOF_VOID_P EQUAL 4)
        message(FATAL_ERROR "The bundled MSVC raylib libraries are x64. Please configure with -A x64.")
    endif()

    if(NOT MSVC AND CMAKE_SIZEOF_VOID_P EQUAL 4)
        message(FATAL_ERROR "The bundled MinGW raylib libraries are x64. Please use a 64-bit toolchain.")
    endif()

    if(MSVC)
        set(RAYLIB_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../2-Pong/thirdparty/raylib/lib/msvc")
        set(RAYLIB_IMPORT_LIB "${RAYLIB_LIB_DIR}/raylibdll.lib")
    else()
        set(RAYLIB_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../2-Pong/thirdparty/raylib/lib/mingw")
        set(RAYLIB_IMPORT_LIB "${RAYLIB_LIB_DIR}/libraylib.dll.a")
    endif()

    set(RAYLIB_RUNTIME_FILE "${RAYLIB_LIB_DIR}/raylib.dll")
    if(NOT MSVC)
        set(RAYLIB_RUNTIME_FILE "${RAYLIB_LIB_DIR}/libraylib.dll")
    endif()

    if(NOT EXISTS "${RAYLIB_INCLUDE_DIR}/raylib.h" OR NOT EXISTS "${RAYLIB_IMPORT_LIB}" OR NOT EXISTS "${RAYLIB_RUNTIME_FILE}")
        message(FATAL_ERROR "The shared raylib files from 2-Pong were not found.")
    endif()

    add_library(raylib SHARED IMPORTED GLOBAL)
    set_target_properties(raylib PROPERTIES
        IMPORTED_IMPLIB "${RAYLIB_IMPORT_LIB}"
        IMPORTED_LOCATION "${RAYLIB_RUNTIME_FILE}"
        INTERFACE_INCLUDE_DIRECTORIES "${RAYLIB_INCLUDE_DIR}"
    )
elseif(APPLE)
    find_path(RAYLIB_INCLUDE_DIR raylib.h HINTS /opt/homebrew /usr/local /opt/local PATH_SUFFIXES include)
    find_library(RAYLIB_LIBRARY raylib HINTS /opt/homebrew /usr/local /opt/local PATH_SUFFIXES lib)
    if(NOT RAYLIB_INCLUDE_DIR OR NOT RAYLIB_LIBRARY)
        message(FATAL_ERROR "macOS raylib was not found. Install it with: brew install raylib")
    endif()
    add_library(raylib UNKNOWN IMPORTED GLOBAL)
    set_target_properties(raylib PROPERTIES
        IMPORTED_LOCATION "${RAYLIB_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${RAYLIB_INCLUDE_DIR}"
    )
elseif(UNIX)
    find_package(PkgConfig QUIET)
    if(PkgConfig_FOUND)
        pkg_check_modules(RAYLIB_PC QUIET IMPORTED_TARGET raylib)
    endif()

    if(RAYLIB_PC_FOUND)
        add_library(raylib INTERFACE IMPORTED GLOBAL)
        target_link_libraries(raylib INTERFACE PkgConfig::RAYLIB_PC)
    else()
        find_path(RAYLIB_INCLUDE_DIR raylib.h)
        find_library(RAYLIB_LIBRARY NAMES raylib libraylib)
        if(NOT RAYLIB_INCLUDE_DIR OR NOT RAYLIB_LIBRARY)
            message(FATAL_ERROR "Linux raylib development files were not found. Install them with: sudo apt install libraylib-dev")
        endif()
        add_library(raylib UNKNOWN IMPORTED GLOBAL)
        set_target_properties(raylib PROPERTIES
            IMPORTED_LOCATION "${RAYLIB_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES "${RAYLIB_INCLUDE_DIR}"
        )
    endif()
else()
    message(FATAL_ERROR "This project supports Windows, macOS, and Linux only.")
endif()

# Change this one path to reference/02.cpp, reference/03.cpp, or reference/04.cpp
# when presenting a later stage.
add_executable(MiniSurvivors "reference/04.cpp")
target_link_libraries(MiniSurvivors PRIVATE raylib)

add_custom_command(TARGET MiniSurvivors POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/res"
        "$<TARGET_FILE_DIR:MiniSurvivors>/res"
)

if(WIN32)
    add_custom_command(TARGET MiniSurvivors POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different
            "${RAYLIB_RUNTIME_FILE}"
            "$<TARGET_FILE_DIR:MiniSurvivors>"
    )
endif()
