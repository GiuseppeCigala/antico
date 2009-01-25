SOURCES = src/main.cpp \
 src/antico.cpp \
 src/frame.cpp \
 src/border.cpp \
 src/dockbar.cpp \
 src/launcher.cpp \
 src/dockicon.cpp \
 src/desk.cpp \
 src/header.cpp \
 src/systray.cpp \
 src/manager.cpp \
 src/filedialog.cpp \
 src/colordialog.cpp \
 src/colorsel.cpp \
 src/msgbox.cpp \
 src/runner.cpp \
 src/dateclock.cpp \
 src/sysicon.cpp \
 src/deskapp.cpp \
 src/utils.cpp \
 src/deskdev.cpp \
 src/deskfile.cpp \
 src/deskfolder.cpp
HEADERS = src/defs.h \
 src/antico.h \
 src/frame.h \
 src/border.h \
 src/dockbar.h \
 src/launcher.h \
 src/dockicon.h \
 src/desk.h \
 src/header.h \
 src/systray.h \
 src/manager.h \
 src/filedialog.h \
 src/colordialog.h \
 src/colorsel.h \
 src/msgbox.h \
 src/runner.h \
 src/dateclock.h \
 src/sysicon.h \
 src/deskapp.h \
 src/utils.h \
 src/deskdev.h \
 src/deskfile.h \
 src/deskfolder.h
OBJECTS_DIR += build
MOC_DIR += build
QMAKE_INCDIR += /usr/include
QMAKE_CLEAN += antico
TEMPLATE = app
CONFIG += warn_on release
QT += dbus
//DEFINES = QT_NO_DEBUG_OUTPUT
