QT += core gui

TARGET = MyPlugin
TEMPLATE        = lib
CONFIG         += plugin

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DEFINES += MYPLUGIN_LIBRARY

# MyPlugin files

SOURCES += \
    myplugin.cpp

HEADERS += \
    myplugin.h

PLUGIN_DIR = $$PWD
INCLUDEPATH += $$PWD/../../
DEPENDPATH += $$PWD/../../
