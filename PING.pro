#-------------------------------------------------
#
# Project created by QtCreator 2014-10-14T15:26:41
#
#-------------------------------------------------

QT       += core gui
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PING
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    PingChunk.cpp \
    PingImage.cpp \
    PingParseError.cpp \
    PingIHDR.cpp \
    PingUtil.cpp

HEADERS  += MainWindow.hpp \
    PingParseError.hpp \
    PingChunk.hpp \
    PingImage.hpp \
    PingIHDR.hpp \
    PingUtil.hpp \
    main.hpp

FORMS    += MainWindow.ui
