cmake_minimum_required(VERSION 3.10.0)

project("Wayland"
    VERSION "0.0.0"
    DESCRIPTION "Wayland client and server extensions"
    LANGUAGES CXX C
)

## Shared macros and functions:
if(NOT LIRI_LOCAL_ECM)
    find_package(LiriCMakeShared "1.0.0" REQUIRED NO_MODULE)
    list(APPEND CMAKE_MODULE_PATH "${LCS_MODULE_PATH}")
endif()

## Add some paths to check for CMake modules:
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake;${CMAKE_CURRENT_SOURCE_DIR}/cmake/3rdparty")

## Set minimum versions required:
set(QT_MIN_VERSION "5.12.0")

## Liri specific setup common for all modules:
include(LiriSetup)

## Find Qt 5:
find_package(Qt5 "${QT_MIN_VERSION}"
    CONFIG REQUIRED
    COMPONENTS
        Core
        Qml
        Quick
        Gui
        WaylandCompositor
        WaylandClient
)

## Add subdirectories:
add_subdirectory(src/waylandclient)
add_subdirectory(src/waylandserver)
add_subdirectory(src/imports/waylandclient)
add_subdirectory(src/imports/waylandserver)
if(BUILD_TESTING)
    if(TARGET WaylandClient)
        add_subdirectory(tests/auto/client)
    endif()
    if(TARGET WaylandServer)
        add_subdirectory(tests/auto/server)
    endif()
endif()
