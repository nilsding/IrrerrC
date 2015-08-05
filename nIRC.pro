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
    src/settings/settingsdialog.cpp \
    src/settings/identitysettingswidget.cpp \
    src/settings/networksettingswidget.cpp

HEADERS  += src/mainwindow.h \
    src/statuswindow.h \
    src/core/ircconnection.h \
    src/core/ircnetwork.h \
    src/core/ircidentity.h \
    src/core/ircparser.h \
    src/core/ircmessage.h \
    src/core/irccodes.h \
    src/core/irctypes.h \
    src/settings/nsettings.h \
    src/settings/settingsdialog.h \
    src/settings/identitysettingswidget.h \
    src/settings/networksettingswidget.h

FORMS    += src/ui/mainwindow.ui \
    src/ui/settings/settingsdialog.ui \
    src/ui/settings/identitysettingswidget.ui \
    src/ui/settings/networksettingswidget.ui

# C++11! \o/
CONFIG += c++11

RESOURCES += \
    nirc.qrc

TRANSLATIONS = lang/nIRC_de.ts

