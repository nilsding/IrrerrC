#-------------------------------------------------
#
# Project created by QtCreator 2015-05-14T12:02:19
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = nIRC
TEMPLATE = app


SOURCES += src/main.cpp \
    src/mainwindow.cpp \
    src/statuswindow.cpp \
    src/core/ircconnection.cpp \
    src/core/ircnetwork.cpp \
    src/core/ircidentity.cpp \
    src/core/ircparser.cpp \
    src/core/ircmessage.cpp \
    src/settings/settingsdialog.cpp

HEADERS  += src/mainwindow.h \
    src/statuswindow.h \
    src/core/ircconnection.h \
    src/core/ircnetwork.h \
    src/core/ircidentity.h \
    src/core/ircparser.h \
    src/core/ircmessage.h \
    src/settings/nsettings.h \
    src/settings/settingsdialog.h

FORMS    += src/ui/mainwindow.ui \
    src/ui/settings/settingsdialog.ui

# C++11! \o/
QMAKE_CXXFLAGS += -std=c++11

RESOURCES += \
    nirc.qrc
