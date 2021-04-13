#-------------------------------------------------
#
# Project created by QtCreator 2020-04-03T17:28:51
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Bjk_backup
TEMPLATE = app

CONFIG += precompile_header
CONFIG   += c++11
PRECOMPILED_HEADER = stable.h

SOURCES += main.cpp\
        FileInfoExtractor.cpp \
        mainwindow.cpp

HEADERS  += mainwindow.h \
    FileInfoExtractor.h

FORMS    += mainwindow.ui
RC_ICONS = 7m.ico
