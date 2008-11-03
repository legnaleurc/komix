# common
TARGET       = komix
DEPENDPATH  += . include include/widget src src/widget
INCLUDEPATH += . include/widget include
MOC_DIR      = moc
RCC_DIR      = rcc
DESTDIR      = bin
CONFIG      += debug_and_release

# Input
HEADERS   += include/widget/imagearea.hpp \
             include/widget/mainwindow.hpp \
             include/widget/scaleimage.hpp
SOURCES   += src/main.cpp \
             src/widget/imagearea.cpp \
             src/widget/mainwindow.cpp \
             src/widget/scaleimage.cpp
RESOURCES  = komix.qrc
QT        += svg

#config
CONFIG( debug, debug|release ) {
	OBJECTS_DIR  = obj/debug
} else {
	OBJECTS_DIR  = obj/release
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
}
