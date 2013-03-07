# -------------------------------------------------
# Project created by QtCreator 2013-10-16T12:16:27
# -------------------------------------------------
TARGET = pose_estimation_visualisation
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    accelerometerwidget.cpp \
    indicator.cpp \
    portwidget.cpp \
    receiver.cpp
HEADERS += mainwindow.h \
    accelerometerwidget.h \
    indicator.h \
    portwidget.h \
    receiver.h
FORMS += mainwindow.ui \
    accelerometerwidget.ui \
    indicator.ui \
    portwidget.ui

# Qextserialport
unix { 
    LIBS += -lqextserialport
    DEFINES = _TTY_POSIX_
}
