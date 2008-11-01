# common
TARGET       = komix
DEPENDPATH  += . src src/widget
INCLUDEPATH += . src/widget src
MOC_DIR      = moc
RCC_DIR      = rcc
DESTDIR      = bin
CONFIG      += debug_and_release

# Input
HEADERS   += src/utility.hpp \
             src/widget/imagearea.hpp \
             src/widget/mainwindow.hpp \
             src/widget/scaleimage.hpp
SOURCES   += src/main.cpp \
             src/utility.cpp \
             src/widget/imagearea.cpp \
             src/widget/mainwindow.cpp \
             src/widget/scaleimage.cpp
RESOURCES  = komix.qrc
QT        += svg

#config
CONFIG( debug, debug|release  ) {
	OBJECTS_DIR  = obj/debug
} else {
	OBJECTS_DIR  = obj/release
	DEFINES     += QT_NO_DEBUG_OUTPUT
}

#platform

unix {
	TEMPLATE = app
}

win32 {
	TEMPLATE = vcapp
	
	CONFIG(debug, release|debug) {
		addPlugins.sources = $$QT_BUILD_TREE/plugins/imageformats/qsvgd4.dll
	}
	CONFIG(release, release|debug) {
		addPlugins.sources = $$QT_BUILD_TREE/plugins/imageformats/qsvg4.dll
	}
	addPlugins.path = imageformats
	DEPLOYMENT += addPlugins
}
