TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    cert4-ecb.cpp \
    exceptn.cpp \
<<<<<<< HEAD \
    gost.cpp \
    gost_tab.cpp \
    mlock.cpp \
    opencl.cpp \
    util.cpp
    gost.cpp \
    gost_tab.cpp \
=======
    gost_tab.cpp \
    gost.cpp \
>>>>>>> a8f8e3691d999c0d444c39eb9d20e597bd91fac8
    mlock.cpp \
    opencl.cpp \
    util.cpp

INCLUDEPATH += \
    ..\
