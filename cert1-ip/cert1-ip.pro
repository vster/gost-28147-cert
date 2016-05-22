TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    exceptn.cpp \
    gost.cpp \
    gost_tab.cpp \
    gost-ip.cpp \
    mlock.cpp \
    opencl.cpp \
    util.cpp

INCLUDEPATH += \
    ..\

HEADERS += \
    gost.h
