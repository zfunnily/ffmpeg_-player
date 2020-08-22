cmake_minimum_required(VERSION 2.8)
project(ffplay)
set(PROJECT_OUTNAME ffplay)


if(CMAKE_TOOLCHAIN_FILE)
    include(${CMAKE_TOOLCHAIN_FILE})
endif(CMAKE_TOOLCHAIN_FILE)

find_package(FFMPEG)
if (FFMPEG_FOUND)
    #set(FFMPEG_INCLUDE_DIRS /usr/local/Cellar/ffmpeg/4.2.2_3/include)
    set(FFMPEG_LIBRARY_PATH /Users/sources/vcpkg/installed/x64-osx/debug/lib/)
else(FFMPEG_FOUND)
    set(FFMPEG_INCLUDE_DIRS /usr/local/Cellar/ffmpeg/4.2.2_3/include)
    set(FFMPEG_LIBRARY_PATH /usr/local/Cellar/ffmpeg/4.2.2_3/lib)
endif ()

#add_definitions(-D__STDC_CONSTANT_MACROS)
add_definitions(-DQT_DEPRECATED_WARNINGS)

IF (WIN32)
    SET(OUTPUT_PATH_DEBUG ${PROJECT_SOURCE_DIR}/../build/debug_windows_x64)
    SET(OUTPUT_PATH_RELEASE ${PROJECT_SOURCE_DIR}/../bin/windows_x64/im_common)
    SET(PROJECT_LIB_DIR_DEBUG ${PROJECT_SOURCE_DIR}/../build/debug_windows_x64)
    SET(PROJECT_LIB_DIR_RELEASE ${PROJECT_SOURCE_DIR}/../bin/windows_x64/im_common)

    SET(PROJECT_LIBS_PLATFORM_DEBUG
        debug ${FFMPEG_LIBRARIES}
        )
    SET(PROJECT_LIBS_PLATFORM_RELEASE
        #optimized ${PROJECT_LIB_DIR_RELEASE}/
        optimized ${FFMPEG_LIBRARIES}
        )

    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
ELSEIF (APPLE)

    SET(OUTPUT_PATH_DEBUG ${PROJECT_SOURCE_DIR}/../build/debug_macos_x64)
    SET(OUTPUT_PATH_RELEASE ${PROJECT_SOURCE_DIR}/../bin/macos_x64/im_common)
    SET(PROJECT_LIB_DIR_DEBUG ${PROJECT_SOURCE_DIR}/../build/debug_macos_x64)
    SET(PROJECT_LIB_DIR_RELEASE ${PROJECT_SOURCE_DIR}/../bin/macos_x64/im_common)

    SET(PROJECT_LIBS_PLATFORM_DEBUG
        #debug ${FFMPEG_LIBRARIES}
        debug X11
        debug dl

        debug ${FFMPEG_LIBRARY_PATH}/libavcodec.dylib
        debug ${FFMPEG_LIBRARY_PATH}/libavdevice.dylib
        debug ${FFMPEG_LIBRARY_PATH}/libavfilter.dylib
        debug ${FFMPEG_LIBRARY_PATH}/libavformat.dylib
        debug ${FFMPEG_LIBRARY_PATH}/libavresample.dylib
        debug ${FFMPEG_LIBRARY_PATH}/libavutil.dylib
        debug ${FFMPEG_LIBRARY_PATH}/libpostproc.dylib
        debug ${FFMPEG_LIBRARY_PATH}/libswresample.dylib
        debug ${FFMPEG_LIBRARY_PATH}/libswscale.dylib
        )
    SET(PROJECT_LIBS_PLATFORM_RELEASE
        #optimized ${PROJECT_LIB_DIR_RELEASE}/
        #optimized ${FFMPEG_LIBRARIES}
        optimized X11
        optimized dl

        optimized ${FFMPEG_LIBRARY_PATH}/libavcodec.dylib
        optimized ${FFMPEG_LIBRARY_PATH}/libavdevice.dylib
        optimized ${FFMPEG_LIBRARY_PATH}/libavfilter.dylib
        optimized ${FFMPEG_LIBRARY_PATH}/libavformat.dylib
        optimized ${FFMPEG_LIBRARY_PATH}/libavresample.dylib
        optimized ${FFMPEG_LIBRARY_PATH}/libavutil.dylib
        optimized ${FFMPEG_LIBRARY_PATH}/libpostproc.dylib
        optimized ${FFMPEG_LIBRARY_PATH}/libswresample.dylib
        optimized ${FFMPEG_LIBRARY_PATH}/libswscale.dylib
        )
    set (LIBX11LIB_PATH /usr/X11/lib/)

ELSEIF (UNIX)
ENDIF ()

include_directories(
        ${PROJECT_SOURCE_DIR}/inc
         ${FFMPEG_INCLUDE_DIRS}
)
link_directories(
    ${FFMPEG_LIBRARY_DIRS}
    ${LIBX11LIB_PATH}
    )

file(GLOB_RECURSE SRC_FILES src/*.cpp)
file(GLOB_RECURSE HEAD_FILES inc/*.h)
file(GLOB_RECURSE ui_files src/*.ui)
file(GLOB_RECURSE res_files res/*.qrc)

QT5_WRAP_CPP(MISTUDIO_HEADERS_MOC ${HEAD_FILES})
QT5_WRAP_UI(MISTUDIO_UIS_UIC ${ui_files})
QT5_ADD_RESOURCES(MISTUDIO_QRC_RCC ${res_files})


ADD_EXECUTABLE(${PROJECT_OUTNAME} ${SRC_FILES} ${HEAD_FILES} ${MISTUDIO_HEADERS_MOC} ${MISTUDIO_UIS_UIC} ${MISTUDIO_QRC_RCC})

target_link_libraries(${PROJECT_OUTNAME}
        PUBLIC Qt5::Core
        PUBLIC Qt5::Gui
        PUBLIC Qt5::Widgets
        PUBLIC Qt5::Network
        PRIVATE ${QSS_LIBRARY_VAR}
        PRIVATE ${QRENCODE_LIBRARY_VAR}
        PRIVATE ${ZBAR_LIBRARY_VAR}
        ${PROJECT_LIBS_PLATFORM_RELEASE}
        ${PROJECT_LIBS_PLATFORM_DEBUG}
        )


