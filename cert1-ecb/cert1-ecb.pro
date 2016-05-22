TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    cert1-ecb.cpp \
    exceptn.cpp \
    gost.cpp \
    gost_tab.cpp \
    mlock.cpp \
    opencl.cpp \
    util.cpp

HEADERS += \
    gost.h

INCLUDEPATH += \
    ..\
