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

# some compile-time defined constants
VERSION = 0.0.1
QMAKE_TARGET_PRODUCT = nIRC
QMAKE_TARGET_DESCRIPTION = An IRC client.
QMAKE_TARGET_COMPANY = nilsding
QMAKE_TARGET_COPYRIGHT = © 2015 nilsding

DEFINES += APP_VERSION=\\\"$$join(VERSION, "\\ ")\\\" \
           APP_NAME=\\\"$$join(QMAKE_TARGET_PRODUCT, "\\ ")\\\" \
           APP_DESCRIPTION=\\\"$$join(QMAKE_TARGET_DESCRIPTION, "\\ ")\\\" \
           APP_COMPANY=\\\"$$join(QMAKE_TARGET_COMPANY, "\\ ")\\\" \
           APP_COPYRIGHT=\\\"$$join(QMAKE_TARGET_COPYRIGHT, "\\ ")\\\"

