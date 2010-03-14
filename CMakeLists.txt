cmake_minimum_required(VERSION 2.8)

project(KomiX)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

set(QT_USE_QTSVG TRUE)
find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})
find_package(QtSingleApplication REQUIRED)
include_directories(${QTSINGLEAPPLICATION_INCLUDE_DIR})

set(KOMIX_VERSION_MAJOR 0)
set(KOMIX_VERSION_MINOR 1)
set(KOMIX_VERSION_PATCH 50)
set(KOMIX_VERSION "${KOMIX_VERSION_MAJOR}.${KOMIX_VERSION_MINOR}.${KOMIX_VERSION_PATCH}")
add_definitions(-DKOMIX_VERSION=${KOMIX_VERSION} -DKOMIX_VERSION_MAJOR=${KOMIX_VERSION_MAJOR} -DKOMIX_VERSION_MINOR=${KOMIX_VERSION_MINOR} -DKOMIX_VERSION_PATCH=${KOMIX_VERSION_PATCH})

include_directories(. src/model src/utility src/widget)
include_directories(${CMAKE_BINARY_DIR})
set(KOMIX_MOC_HEADERS
    src/model/archive/archivehook.hpp
    src/model/directory/directoryhook.hpp
    src/model/single/singlehook.hpp
    src/utility/filecontroller.hpp
    src/widget/imagearea.hpp
    src/widget/imageview.hpp
    src/widget/mainwindow.hpp
    src/widget/navigator.hpp
    src/widget/preference.hpp
    src/widget/scalewidget.hpp)
file(GLOB_RECURSE KOMIX_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} src/*.cpp)
file(GLOB_RECURSE KOMIX_HEADERS RELATIVE ${CMAKE_SOURCE_DIR} src/*.hpp)
file(GLOB_RECURSE KOMIX_FORMS RELATIVE ${CMAKE_SOURCE_DIR} src/*.ui)
file(GLOB_RECURSE KOMIX_TRANSLATIONS RELATIVE ${CMAKE_SOURCE_DIR} translations/*.ts)
set(KOMIX_RESOURCES komix.qrc)

qt4_wrap_cpp(KOMIX_MOC_SOURCES ${KOMIX_MOC_HEADERS})
qt4_add_resources(KOMIX_RCC_RESOURCES ${KOMIX_RESOURCES})
qt4_wrap_ui(KOMIX_UIC_HEADERS ${KOMIX_FORMS})
qt4_add_translation(KOMIX_QM_FILES ${KOMIX_TRANSLATIONS})

source_group("Generated Files" FILES ${KOMIX_MOC_SOURCES} ${KOMIX_RCC_RESOURCES} ${KOMIX_UIC_HEADERS} ${KOMIX_QM_FILES})
source_group("Resource Files" REGULAR_EXPRESSION .*\\.rc)
source_group("Form Files" REGULAR_EXPRESSION .*\\.ui)

if(QT_STATIC)
    find_library(QT_GIF_PLUGIN qgif PATHS ${QT_PLUGINS_DIR} PATH_SUFFIXES imageformats)
    set(KOMIX_EXTRA_LIBRARIES ${KOMIX_EXTRA_LIBRARIES} ${QT_GIF_PLUGIN})
    find_library(QT_ICO_PLUGIN qico PATHS ${QT_PLUGINS_DIR} PATH_SUFFIXES imageformats)
    set(KOMIX_EXTRA_LIBRARIES ${KOMIX_EXTRA_LIBRARIES} ${QT_ICO_PLUGIN})
    find_library(QT_JPEG_PLUGIN qjpeg PATHS ${QT_PLUGINS_DIR} PATH_SUFFIXES imageformats)
    set(KOMIX_EXTRA_LIBRARIES ${KOMIX_EXTRA_LIBRARIES} ${QT_JPEG_PLUGIN})
    find_library(QT_MNG_PLUGIN qmng PATHS ${QT_PLUGINS_DIR} PATH_SUFFIXES imageformats)
    set(KOMIX_EXTRA_LIBRARIES ${KOMIX_EXTRA_LIBRARIES} ${QT_MNG_PLUGIN})
    find_library(QT_SVG_PLUGIN qsvg PATHS ${QT_PLUGINS_DIR} PATH_SUFFIXES imageformats)
    set(KOMIX_EXTRA_LIBRARIES ${KOMIX_EXTRA_LIBRARIES} ${QT_SVG_PLUGIN})
    find_library(QT_TIFF_PLUGIN qtiff PATHS ${QT_PLUGINS_DIR} PATH_SUFFIXES imageformats)
    set(KOMIX_EXTRA_LIBRARIES ${KOMIX_EXTRA_LIBRARIES} ${QT_TIFF_PLUGIN})
    set(QT_USE_QTPLUGIN TRUE)
    add_definitions(-DKOMIX_STATIC)
    if(NOT WIN32)
        set(KOMIX_EXTRA_LIBRARIES ${KOMIX_EXTRA_LIBRARIES} jpeg mng)
    endif()
endif()

set(KOMIX_COMPILE_UNITS ${KOMIX_SOURCES} ${KOMIX_MOC_SOURCES} ${KOMIX_RCC_RESOURCES} ${KOMIX_QM_FILES})

if(WIN32)
    add_executable(komix WIN32 komix.rc ${KOMIX_HEADERS} ${KOMIX_UIC_HEADERS} ${KOMIX_COMPILE_UNITS})
    set(KOMIX_EXTRA_LIBRARIES ${KOMIX_EXTRA_LIBRARIES} ${QT_QTMAIN_LIBRARY})
else()
    add_executable(komix ${KOMIX_COMPILE_UNITS})
endif()

target_link_libraries(komix ${KOMIX_EXTRA_LIBRARIES} ${QT_LIBRARIES} ${QTSINGLEAPPLICATION_LIBRARY})

if(WIN32)
    install(TARGETS komix
        RUNTIME DESTINATION .)
else()
    install(TARGETS komix
        RUNTIME DESTINATION bin)
endif()
