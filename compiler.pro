QT += core
QT -= gui

TARGET = compiler
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    fd.cpp

HEADERS += \
    fd.h

CONFIG += c++11

DISTFILES += \
    test.txt
