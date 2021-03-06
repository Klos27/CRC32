#-------------------------------------------------
#
# Project created by QtCreator 2018-05-21T01:17:34
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CRC32
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
CONFIG += c++11

INCLUDEPATH += D:/Programy/boost_1_67_0/
LIBS += "-LC:/boost_1_53_0/stage/lib/"

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    crc.cpp \
    crc_old.cpp

HEADERS += \
        mainwindow.h \
    crc.h \
    crc_old.h

FORMS += \
        mainwindow.ui
