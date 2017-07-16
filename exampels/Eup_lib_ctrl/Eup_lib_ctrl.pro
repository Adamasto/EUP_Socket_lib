#-------------------------------------------------
#
# Project created by QtCreator 2017-07-12T10:51:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Eup_lib_ctrl
TEMPLATE = app
#TEMPLATE = lib
CONFIG += staticlib c++11

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

MOC_DIR = moc
OBJECTS_DIR = obj

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../src/SNetWork/release/ -lSNetWork
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../src/SNetWork/debug/ -lSNetWork
else:unix:!macx: LIBS += -L$$OUT_PWD/../../src/SNetWork/ -lSNetWork

INCLUDEPATH += $$PWD/../../src/SNetWork \

DEPENDPATH += $$PWD/../..//src/SNetWork \



SOURCES += \
        main.cpp \
        MainWindow.cpp

HEADERS += \
        MainWindow.h

FORMS += \
        MainWindow.ui

LIBS += -lsetupapi
LIBS += -lws2_32
