# Uncomment this line to enable static build
# CONFIG += komix_static
# Common
TEMPLATE = app
TARGE T = komix
CONFIG += debug_and_release \
    warn_all
QT += svg

# Input
DEPENDPATH += . \
    src \
    src/utility \
    src/widget
INCLUDEPATH += $${DEPENDPATH}
HEADERS += src/utility/filecontroller.hpp \
    src/utility/filemodel.hpp \
    src/utility/global.hpp \
    src/widget/imagearea.hpp \
    src/widget/mainwindow.hpp \
    src/widget/preview.hpp \
    src/widget/scaleimage.hpp \
    src/utility/archivemodel.hpp \
    src/utility/directorymodel.hpp \
    src/utility/singlemodel.hpp
SOURCES += src/utility/filecontroller.cpp \
    src/utility/filemodel.cpp \
    src/utility/global.cpp \
    src/main.cpp \
    src/widget/imagearea.cpp \
    src/widget/mainwindow.cpp \
    src/widget/preview.cpp \
    src/widget/scaleimage.cpp \
    src/utility/archivemodel.cpp \
    src/utility/directorymodel.cpp \
    src/utility/singlemodel.cpp
RESOURCES = komix.qrc

# Ouput
MOC_DIR = tmp/moc
RCC_DIR = tmp/rcc
DESTDIR = bin

# Special
CONFIG( debug, debug|release ):OBJECTS_DIR = tmp/obj/debug
else { 
    OBJECTS_DIR = tmp/obj/release
    DEFINES += QT_NO_DEBUG_OUTPUT
}
komix_static { 
    QTPLUGIN += qgif \
        qico \
        qjpeg \
        qmng \
        qsvg \
        qtiff
    DEFINES += KOMIX_STATIC
    unix:LIBS += -ljpeg \
        -lmng
}

# Platform
unix:CONFIG( release, debug|release ):QMAKE_POST_LINK = strip \
    $(TARGET)
win32 { 
    CONFIG += embed_manifest_exe
    CONFIG( debug, debug|release ):CONFIG += console
}
