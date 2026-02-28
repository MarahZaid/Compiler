QT -= gui
CONFIG += console
CONFIG -= app_bundle

TARGET = compiler
TEMPLATE = app

SOURCES += \
    main.cpp \
    fd.cpp \
    scanner.cpp

HEADERS += \
    fd.h \
    scanner.h

CONFIG += c++17

DISTFILES += \
    test.txt \
    test_scanner.txt \
    regex.txt \
