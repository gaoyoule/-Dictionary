#-------------------------------------------------
#
# Project created by QtCreator 2023-11-21T14:27:28
#
#-------------------------------------------------

QT       += core gui
QT += network
QT += sql
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Dict_Server
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    tcpserver.cpp \
    CJsonObject.cpp \
    cJSON.c \
    doclientthread.cpp \
    daofactory.cpp \
    userinfodao.cpp \
    worddao.cpp \
    historyinfodao.cpp \
    wordbookdao.cpp

HEADERS  += mainwindow.h \
    tcpserver.h \
    cJSON.h \
    CJsonObject.hpp \
    doclientthread.h \
    daofactory.h \
    userinfodao.h \
    worddao.h \
    historyinfodao.h \
    wordbookdao.h

FORMS    += mainwindow.ui

RESOURCES += \
    res.qrc
