cmake_minimum_required(VERSION 2.8)
project(ffplay)
set(PROJECT_OUTNAME ffplay)


if(CMAKE_TOOLCHAIN_FILE)
    include(${CMAKE_TOOLCHAIN_FILE})
endif(CMAKE_TOOLCHAIN_FILE)

find_package(FFMPEG)
if (FFMPEG_FOUND)
endif ()

message("############" ${FFMPEG_INCLUDE_DIRS})
    message("############" ${FFMPEG_LIBRARY_DIRS})

SET(OUTPUT_PATH_DEBUG ${PROJECT_SOURCE_DIR}/../build/debug_macos_x64)
SET(OUTPUT_PATH_RELEASE ${PROJECT_SOURCE_DIR}/../bin/macos_x64/im_common)
SET(PROJECT_LIB_DIR_DEBUG ${PROJECT_SOURCE_DIR}/../build/debug_macos_x64)
SET(PROJECT_LIB_DIR_RELEASE ${PROJECT_SOURCE_DIR}/../bin/macos_x64/im_common)

SET(PROJECT_LIBS_PLATFORM_DEBUG
    debug ${FFMPEG_LIBRARIES}
    debug X11
    debug dl
    )
SET(PROJECT_LIBS_PLATFORM_RELEASE
    #optimized ${PROJECT_LIB_DIR_RELEASE}/
    optimized ${FFMPEG_LIBRARIES}
    )


include_directories(
        ${PROJECT_SOURCE_DIR}/inc
         ${FFMPEG_INCLUDE_DIRS}
)
link_directories(
    ${FFMPEG_LIBRARY_DIRS}
    /usr/X11/lib/
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


