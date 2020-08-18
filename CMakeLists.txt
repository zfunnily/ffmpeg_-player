# vcpkg的根目录
set(CMAKE_TOOLCHAIN_FILE "/Users/sources/vcpkg/scripts/buildsystems/vcpkg.cmake")
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

#set(QT Core Gui Widgets Network DBus Sql)
#find_package(Qt5 REQUIRED ${QT})
#qt5_use_modules(${PROJECT_OUTNAME} ${QT})

#pkg_check_modules(QSS REQUIRED QtShadowsocks>=2.0.0)
#find_library(QSS_LIBRARY_VAR
      #  NAMES ${QSS_LIBRARIES}
      #  HINTS ${QSS_LIBRARY_DIRS} ${QSS_LIBDIR})

if(WIN32 OR APPLE)
    add_definitions(-DFD_SETSIZE=1024)
endif()
add_subdirectory(ffplay)
