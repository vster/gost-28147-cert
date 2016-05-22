TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    exceptn.cpp \
    gost.cpp \
    gost_tab.cpp \
    mlock.cpp \
    opencl.cpp \
    util.cpp \
    cert1-cfb.cpp

HEADERS += \
    gost.h

INCLUDEPATH += \
    ..\
