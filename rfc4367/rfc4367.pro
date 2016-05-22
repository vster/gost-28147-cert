TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    exceptn.cpp \
    gmodes.cpp \
    gost.cpp \
    gost_tab.cpp \
    gost-main.cpp \
    mlock.cpp \
    opencl.cpp \
    util.cpp

HEADERS += \
    gmodes.h \
    vchtest.h

INCLUDEPATH += \
    ..\
