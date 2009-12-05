cmake_minimum_required(VERSION 2.8)

project(KomiX)

set(QT_USE_QTSVG TRUE)
find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})

set(KOMIX_VERSION 0.1.0)
add_definitions(-DKOMIX_VERSION=${KOMIX_VERSION})

include_directories(. src/model src/utility src/widget)
set(KOMIX_MOC_HEADERS
    src/model/archive/archivehook.hpp
    src/model/directory/directoryhook.hpp
    src/model/single/singlehook.hpp
    src/utility/filecontroller.hpp
    src/widget/imagearea.hpp
    src/widget/mainwindow.hpp
    src/widget/navigator.hpp
    src/widget/preference.hpp
    src/widget/scalepanel.hpp)
set(KOMIX_SOURCES
    src/model/archive/archivehook.cpp
    src/model/archive/archivemodel.cpp
    src/model/directory/directoryhook.cpp
    src/model/directory/directorymodel.cpp
    src/model/single/singlehook.cpp
    src/model/single/singlemodel.cpp
    src/model/filemodel.cpp
    src/model/localfilemodel.cpp
    src/utility/error.cpp
    src/utility/filecontroller.cpp
    src/utility/global.cpp
    src/widget/imagearea.cpp
    src/widget/mainwindow.cpp
    src/widget/navigator.cpp
    src/widget/preference.cpp
    src/widget/scalepanel.cpp
    src/main.cpp)
set(KOMIX_RESOURCES komix.qrc)

qt4_wrap_cpp(KOMIX_MOC_SOURCES ${KOMIX_MOC_HEADERS})
qt4_add_resources(KOMIX_RCC_RESOURCES ${KOMIX_RESOURCES})

if(QT_STATIC)
    find_library(QT_GIF_PLUGIN qgif PATHS ${QT_PLUGINS_DIR} PATH_SUFFIXES imageformats)
    find_library(QT_GIF_PLUGIN qico PATHS ${QT_PLUGINS_DIR} PATH_SUFFIXES imageformats)
    find_library(QT_GIF_PLUGIN qjpeg PATHS ${QT_PLUGINS_DIR} PATH_SUFFIXES imageformats)
    find_library(QT_GIF_PLUGIN qmng PATHS ${QT_PLUGINS_DIR} PATH_SUFFIXES imageformats)
    find_library(QT_GIF_PLUGIN qsvg PATHS ${QT_PLUGINS_DIR} PATH_SUFFIXES imageformats)
    find_library(QT_GIF_PLUGIN qtiff PATHS ${QT_PLUGINS_DIR} PATH_SUFFIXES imageformats)
    add_definitions(-DKOMIX_STATIC)
    if(NOT WIN32)
        set(KOMIX_EXTRA_LIBRARIES ${KOMIX_EXTRA_LIBRARIES} jpeg mng)
    endif()
endif()

if(WIN32)
    add_executable(komix WIN32 komix.rc ${KOMIX_SOURCES} ${KOMIX_MOC_SOURCES} ${KOMIX_RCC_RESOURCES})
    set(KOMIX_EXTRA_LIBRARIES ${KOMIX_EXTRA_LIBRARIES} ${QT_QTMAIN_LIBRARY})
else()
    add_executable(komix ${KOMIX_SOURCES} ${KOMIX_MOC_SOURCES} ${KOMIX_RCC_RESOURCES})
endif()

target_link_libraries(komix ${KOMIX_EXTRA_LIBRARIES} ${QT_LIBRARIES})

install(TARGETS komix
    RUNTIME DESTINATION bin)
