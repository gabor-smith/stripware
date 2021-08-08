#-------------------------------------------------
#
# Project created by QtCreator 2021-08-04T21:01:09
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Stripware
TEMPLATE = app

VERSION = 2.0

DEFINES += APP_VERSION=\\\"$$VERSION\\\"

SOURCES += main.cpp \
    aboutdialog.cpp \
    mainwindow.cpp

HEADERS  += \
    aboutdialog.h \
    mainwindow.h

FORMS    += \
    aboutdialog.ui \
    mainwindow.ui

DISTFILES +=

RESOURCES += \
    res.qrc
