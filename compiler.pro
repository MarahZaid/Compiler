QT -= gui
CONFIG += console
CONFIG -= app_bundle

TARGET = compiler
TEMPLATE = app

SOURCES += \
    main.cpp \
    fd.cpp \
    scanner.cpp \
    token_utils.cpp

HEADERS += \
    fd.h \
    scanner.h \
    token_utils.h

CONFIG += c++17

DISTFILES += \
    test.txt \
    test_scanner.txt \
    Task1_REs.txt
