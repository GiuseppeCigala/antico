SOURCES = src/main.cpp \
 src/device.cpp \
 src/mixer.cpp \
 src/extension.cpp \
 src/vumeter.cpp \
 src/slider.cpp \
 src/group.cpp \
 src/mute.cpp \
 src/onoff.cpp \
 src/enum.cpp \
 src/peak.cpp \
 src/push.cpp \
 src/cursor.cpp \
 src/info.cpp
HEADERS = src/defs.h \
 src/device.h \
 src/mixer.h \
 src/extension.h \
 src/vumeter.h \
 src/slider.h \
 src/group.h \
 src/mute.h \
 src/onoff.h \
 src/enum.h \
 src/peak.h \
 src/push.h \
 src/cursor.h \
 src/info.h
OBJECTS_DIR += build
MOC_DIR += build
QMAKE_INCDIR += /usr/include
QMAKE_CLEAN += mixer
TEMPLATE = app
CONFIG += warn_on release
//LIBS += -L/usr/lib/oss/lib -lOSSlib
