TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    cert4-ecb.cpp \
    exceptn.cpp \
    gost_tab.cpp \
    gost.cpp \
    mlock.cpp \
    opencl.cpp \
    util.cpp

INCLUDEPATH += \
    ..\
