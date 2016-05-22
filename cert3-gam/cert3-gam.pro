TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    cert3-gam.cpp \
    exceptn.cpp \
    gost.cpp \
    gost_tab.cpp \
    mlock.cpp \
    opencl.cpp \
    util.cpp

INCLUDEPATH += \
    ..\
