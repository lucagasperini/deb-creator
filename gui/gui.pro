#-------------------------------------------------
#
# Project created by QtCreator 2018-09-01T12:17:12
#
#-------------------------------------------------

QT       += core widgets sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


TEMPLATE = app
TARGET = debcreator

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    depend.cpp \
    depend_add.cpp \
    build_editor.cpp

HEADERS += \
        mainwindow.h \
        about.h \
    depend.h \
    depend_add.h \
    build_editor.h

FORMS += \
        mainwindow.ui \
    depend.ui \
    depend_add.ui \
    build_editor.ui

target.path = /usr/bin/
icon.path = /usr/share/pixmaps/debcreator.png
icon.files = $$PWD/resource/debcreator.png
desktop.path = /usr/share/applications/
desktop.files = $$PWD/system/debcreator.desktop

INSTALLS += target icon desktop

unix|win32: LIBS += -L$$OUT_PWD/../lib/ -ldebcreator

INCLUDEPATH += $$PWD/../lib
DEPENDPATH += $$PWD/../lib

RESOURCES += \
    resource.qrc
