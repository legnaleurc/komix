# common
TARGET       = komix
DEPENDPATH  += . include include/utility include/widget src src/utility src/widget
INCLUDEPATH += . include include/utility include/widget
MOC_DIR      = tmp/moc
RCC_DIR      = tmp/rcc
DESTDIR      = bin
CONFIG      += debug_and_release

# Input
HEADERS   += include/utility/filecontroller.hpp \
             include/utility/global.hpp \
             include/widget/imagearea.hpp \
             include/widget/mainwindow.hpp \
             include/widget/preview.hpp \
             include/widget/scaleimage.hpp
SOURCES   += src/utility/filecontroller.cpp \
             src/utility/global.cpp \
             src/main.cpp \
             src/widget/imagearea.cpp \
             src/widget/mainwindow.cpp \
             src/widget/preview.cpp \
             src/widget/scaleimage.cpp
RESOURCES  = komix.qrc
QT        += svg

#config
CONFIG( debug, debug|release ) {
	OBJECTS_DIR  = tmp/obj/debug
} else {
	OBJECTS_DIR  = tmp/obj/release
	DEFINES     += QT_NO_DEBUG_OUTPUT
}

#platform
unix {
	TEMPLATE = app
	CONFIG( release, debug|release ) {
		QMAKE_POST_LINK=strip $(TARGET)
	}
}
win32 {
	TEMPLATE = vcapp
	CONFIG += embed_manifest_exe
	QTPLUGIN += qgif \
	            qico \
	            qjpeg \
	            qmng \
	            qsvg \
	            qtiff
	CONFIG( debug, debug|release ) {
		CONFIG += console
	}
}
