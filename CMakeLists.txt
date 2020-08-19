# vcpkg的根目录
IF (WIN32)
SET(VCPKGROOT "D:/Work/vcpkg")
ELSEIF (APPLE)
SET(VCPKGROOT "/Users/sources/vcpkg")
ELSEIF (UNIX)
SET(VCPKGROOT "/Users/sources/vcpkg")
ENDIF ()
set(CMAKE_TOOLCHAIN_FILE "${VCPKGROOT}/scripts/buildsystems/vcpkg.cmake")
cmake_minimum_required(VERSION 2.8)

project(FFPlay
        LANGUAGES CXX)

# 自动调用moc，uic，rcc处理qt的扩展部分
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(APPLE AND EXISTS /usr/local/opt/qt5)
        list(APPEND CMAKE_PREFIX_PATH "/usr/local/opt/qt5")
endif()

find_package(Qt5Core)
find_package(Qt5Gui)
find_package(Qt5Widgets)
find_package(Qt5Network)

if(UNIX AND NOT APPLE)
    find_package(Qt5DBus)
endif()
find_package(PkgConfig)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")


if(WIN32 OR APPLE)
    add_definitions(-DFD_SETSIZE=1024)
endif()
add_subdirectory(ffplay)
