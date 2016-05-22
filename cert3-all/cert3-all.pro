TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    cert3-all.cpp \
    exceptn.cpp \
    gmodes.cpp \
    gost.cpp \
    gost_tab.cpp \
    mlock.cpp \
    opencl.cpp \
    util.cpp

HEADERS += \
    gmodes.h \
    vchtest.h

INCLUDEPATH += \
    ..\
