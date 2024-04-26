#-------------------------------------------------
#
# Project created by QtCreator 2023-11-22T09:16:24
#
#-------------------------------------------------

QT       += core gui
QT += network
QT += sql
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Dict_Client
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    CJsonObject.cpp \
    cJSON.c \
    searchword.cpp

HEADERS  += mainwindow.h \
    cJSON.h \
    CJsonObject.hpp \
    searchword.h

FORMS    += mainwindow.ui \
    searchword.ui

RESOURCES += \
    res.qrc
