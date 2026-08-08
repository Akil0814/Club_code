cmake_minimum_required(VERSION 3.20)
project(RaylibTemplate LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/utf-8)
endif()

if(WIN32)
    set(RAYLIB_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/raylib/include")

    if(MSVC AND CMAKE_SIZEOF_VOID_P EQUAL 4)
        message(FATAL_ERROR
            "The bundled MSVC raylib libraries are x64, but the current generator platform is Win32. "
            "Please reconfigure CMake with a 64-bit generator, for example: cmake -S . -B build -A x64"
        )
    endif()

    if(NOT MSVC AND CMAKE_SIZEOF_VOID_P EQUAL 4)
        message(FATAL_ERROR
            "The bundled MinGW raylib libraries are x64, but the current toolchain is 32-bit. "
            "Please reconfigure CMake with a 64-bit MinGW toolchain."
        )
    endif()

    if(NOT EXISTS "${RAYLIB_INCLUDE_DIR}/raylib.h")
        message(FATAL_ERROR "Bundled raylib headers were not found in: ${RAYLIB_INCLUDE_DIR}")
    endif()

    include_directories(SYSTEM ${RAYLIB_INCLUDE_DIR})

    if(MSVC)
        set(RAYLIB_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/raylib/lib/msvc")
        if(EXISTS "${RAYLIB_LIB_DIR}/raylibdll.lib")
            set(RAYLIB_IMPORT_LIB "${RAYLIB_LIB_DIR}/raylibdll.lib")
        else()
            set(RAYLIB_IMPORT_LIB "${RAYLIB_LIB_DIR}/raylib.lib")
        endif()
        set(RAYLIB_RUNTIME_FILE "${RAYLIB_LIB_DIR}/raylib.dll")
    else()
        set(RAYLIB_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/raylib/lib/mingw")
        set(RAYLIB_IMPORT_LIB "${RAYLIB_LIB_DIR}/libraylib.dll.a")
        set(RAYLIB_RUNTIME_FILE "${RAYLIB_LIB_DIR}/libraylib.dll")
    endif()

    if(NOT EXISTS "${RAYLIB_IMPORT_LIB}")
        message(FATAL_ERROR "Bundled raylib import library was not found: ${RAYLIB_IMPORT_LIB}")
    endif()

    if(NOT EXISTS "${RAYLIB_RUNTIME_FILE}")
        message(FATAL_ERROR "Bundled raylib runtime DLL was not found: ${RAYLIB_RUNTIME_FILE}")
    endif()

    add_library(raylib SHARED IMPORTED GLOBAL)
    set_target_properties(raylib PROPERTIES
        IMPORTED_IMPLIB "${RAYLIB_IMPORT_LIB}"
        IMPORTED_LOCATION "${RAYLIB_RUNTIME_FILE}"
        INTERFACE_INCLUDE_DIRECTORIES "${RAYLIB_INCLUDE_DIR}"
    )
elseif(APPLE)
    set(RAYLIB_SEARCH_ROOTS
        /opt/homebrew
        /usr/local
        /opt/local
    )

    find_path(
        RAYLIB_INCLUDE_DIR
        raylib.h
        HINTS ${RAYLIB_SEARCH_ROOTS}
        PATH_SUFFIXES include
        REQUIRED
    )

    find_library(
        RAYLIB_LIBRARY
        raylib
        HINTS ${RAYLIB_SEARCH_ROOTS}
        PATH_SUFFIXES lib
        REQUIRED
    )

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
            message(FATAL_ERROR
                "Linux raylib development files were not found. "
                "On Ubuntu/Debian install them with: sudo apt install libraylib-dev"
            )
        endif()

        add_library(raylib UNKNOWN IMPORTED GLOBAL)
        set_target_properties(raylib PROPERTIES
            IMPORTED_LOCATION "${RAYLIB_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES "${RAYLIB_INCLUDE_DIR}"
        )
    endif()
else()
    message(FATAL_ERROR "This template currently supports Windows, macOS, and Linux only.")
endif()


#add_executable(RaylibTemplate "reference/01.cpp")
#add_executable(RaylibTemplate "reference/02.cpp")
#add_executable(RaylibTemplate "reference/03.cpp")
#add_executable(RaylibTemplate "reference/04.cpp")
add_executable(RaylibTemplate "reference/05.cpp")
#add_executable(RaylibTemplate "main.cpp")

target_link_libraries(RaylibTemplate PRIVATE raylib)

if(WIN32)
    add_custom_command(TARGET RaylibTemplate POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different
            "${RAYLIB_RUNTIME_FILE}"
            "$<TARGET_FILE_DIR:RaylibTemplate>"
    )
endif()
