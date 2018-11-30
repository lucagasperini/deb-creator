#-------------------------------------------------
#
# Project created by QtCreator 2018-09-01T12:17:12
#
#-------------------------------------------------

QT       += core sql

QT       -= gui

TEMPLATE = lib

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

HEADERS += \
    debcreator.h \
    define.h \
    multiprocess.h \
    apt.h \
    package.h \
    changelog.h \
    git.h \
    filesystem.h \
    database.h \
    build_step.h \
    package_list.h \
    dpkg.h \
    changelog_list.h

SOURCES += \
    multiprocess.cpp \
    apt.cpp \
    package.cpp \
    changelog.cpp \
    git.cpp \
    filesystem.cpp \
    database.cpp \
    build_step.cpp \
    package_list.cpp \
    dpkg.cpp \
    changelog_list.cpp

target.path = /usr/lib/

INSTALLS += target
