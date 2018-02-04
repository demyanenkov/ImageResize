#-------------------------------------------------
#
# Project created by QtCreator 2010-09-01T13:37:28
#
#-------------------------------------------------

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ImgResize
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
    imageedit.cpp

HEADERS  += dialog.h \
    imageedit.h

FORMS    += dialog.ui

OTHER_FILES += \
    ImageResize.rc

RESOURCES += \
    res.qrc
    
RC_FILE = ImageResize.rc
