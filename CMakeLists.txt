cmake_minimum_required(VERSION 2.8)

project(KomiX)

set(QT_USE_QTSVG TRUE)
find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})

option(KOMIX_STATIC "Build static version of KomiX" OFF)

include_directories(. src/model src/utility src/widget)
set(KOMIX_HEADERS
    src/model/archive/archivehook.hpp
    src/model/archive/archivemodel.hpp
    src/model/directory/directoryhook.hpp
    src/model/directory/directorymodel.hpp
    src/model/single/singlehook.hpp
    src/model/single/singlemodel.hpp
    src/model/filemodel.hpp
    src/model/localfilemodel.hpp
    src/utility/filecontroller.hpp
    src/utility/global.hpp
    src/utility/error.hpp
    src/widget/imagearea.hpp
    src/widget/mainwindow.hpp
    src/widget/navigator.hpp
    src/widget/preference.hpp
    src/widget/scalepanel.hpp)
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
    src/utility/filecontroller.cpp
    src/utility/global.cpp
    src/utility/error.cpp
    src/widget/imagearea.cpp
    src/widget/mainwindow.cpp
    src/widget/navigator.cpp
    src/widget/preference.cpp
    src/widget/scalepanel.cpp
    src/main.cpp)
set(KOMIX_RESOURCES komix.qrc)

qt4_wrap_cpp(KOMIX_MOC_SOURCES ${KOMIX_MOC_HEADERS})
qt4_add_resources(KOMIX_RCC_RESOURCES ${KOMIX_RESOURCES})
add_executable(komix ${KOMIX_SOURCES} ${KOMIX_MOC_SOURCES} ${KOMIX_RCC_RESOURCES})
target_link_libraries(komix ${QT_LIBRARIES})
