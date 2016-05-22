TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    gmodes.cpp \
    gost.cpp \
    util.cpp

OTHER_FILES += \
    pmkinfo.txt

HEADERS += \
    gmodes.h \
    gost.h \
    util.h \
    vchtest.h

