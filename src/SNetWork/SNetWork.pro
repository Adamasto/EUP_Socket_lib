TEMPLATE = lib
CONFIG += staticlib c++11
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    UdpSocket.cpp

HEADERS += \
    eup_detect_os.h \
    UdpSocket.h
