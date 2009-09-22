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
    src/widget \
    src/model \
    src/model/archive \
    src/model/directory \
    src/model/single
INCLUDEPATH += $${DEPENDPATH}
HEADERS += src/utility/filecontroller.hpp \
    src/model/filemodel.hpp \
    src/utility/global.hpp \
    src/utility/error.hpp \
    src/widget/imagearea.hpp \
    src/widget/mainwindow.hpp \
    src/widget/preview.hpp \
    src/widget/scaleimage.hpp \
    src/model/archive/archivemodel.hpp \
    src/model/directory/directorymodel.hpp \
    src/model/single/singlemodel.hpp \
    src/model/archive/archivehook.hpp \
    src/model/directory/directoryhook.hpp \
    src/model/single/singlehook.hpp \
    src/model/localfilemodel.hpp
SOURCES += src/utility/filecontroller.cpp \
    src/model/filemodel.cpp \
    src/utility/global.cpp \
    src/utility/error.cpp \
    src/main.cpp \
    src/widget/imagearea.cpp \
    src/widget/mainwindow.cpp \
    src/widget/preview.cpp \
    src/widget/scaleimage.cpp \
    src/model/archive/archivemodel.cpp \
    src/model/directory/directorymodel.cpp \
    src/model/single/singlemodel.cpp \
    src/model/archive/archivehook.cpp \
    src/model/directory/directoryhook.cpp \
    src/model/single/singlehook.cpp \
    src/model/localfilemodel.cpp
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
    RC_FILE = komix.rc
}
