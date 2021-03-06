#-------------------------------------------------
#
# Project created by QtCreator 2015-05-14T12:02:19
#
#-------------------------------------------------

CONFIG += scripting

QT       += core gui widgets network svg

TARGET = IrrerrC
TEMPLATE = app

INCLUDEPATH += src/

SOURCES += src/main.cpp \
    src/mainwindow.cpp \
    src/statuswindow.cpp \
    src/aboutdialog.cpp \
    src/core/ircconnection.cpp \
    src/core/ircnetwork.cpp \
    src/core/ircidentity.cpp \
    src/core/ircparser.cpp \
    src/core/ircmessage.cpp \
    src/settings/settingsdialog.cpp \
    src/settings/identitysettingswidget.cpp \
    src/settings/networksettingswidget.cpp \
    src/settings/appearancesettingswidget.cpp \
    src/settings/aliasessettingswidget.cpp \
    src/settings/editaliasdialog.cpp \
    src/util/irctextformatter.cpp \
    src/util/ircalias.cpp \
    src/util/nclickablelabel.cpp

HEADERS  += src/mainwindow.h \
    src/statuswindow.h \
    src/aboutdialog.h \
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
    src/settings/networksettingswidget.h \
    src/settings/appearancesettingswidget.h \
    src/settings/aliasessettingswidget.h \
    src/settings/editaliasdialog.h \
    src/util/irctextformatter.h \
    src/util/ircalias.h \
    src/util/nclickablelabel.h

FORMS    += src/ui/mainwindow.ui \
    src/ui/aboutdialog.ui \
    src/ui/settings/settingsdialog.ui \
    src/ui/settings/identitysettingswidget.ui \
    src/ui/settings/networksettingswidget.ui \
    src/ui/settings/appearancesettingswidget.ui \
    src/ui/settings/aliasessettingswidget.ui \
    src/ui/settings/editaliasdialog.ui

scripting {
    lessThan(QT_MAJOR_VERSION, 5) lessThan(QT_MINOR_VERSION, 6): error("IrrerrC requires Qt 5.6 or newer for scripting")
    QT += qml
    SOURCES += src/script/njsengine.cpp \
        src/script/nscript.cpp \
        src/script/nscriptaction.cpp \
        src/script/nscriptbindings.cpp \
        src/script/nscriptutils.cpp \
        src/script/scriptlistingdialog.cpp
    HEADERS += src/script/njsengine.h \
        src/script/nscript.h \
        src/script/nscriptaction.h \
        src/script/nscriptbindings.h \
        src/script/nscriptutils.h \
        src/script/scriptlistingdialog.h
    FORMS += src/ui/script/scriptlistingdialog.ui
    DEFINES += SCRIPTING_ENABLED
}

# C++11! \o/
CONFIG += c++11

RESOURCES += \
    irrerrc.qrc

RC_FILE = irrerrc.rc

TRANSLATIONS = lang/IrrerrC_de.ts

# some compile-time defined constants
VERSION = 0.0.1
QMAKE_TARGET_PRODUCT = IrrerrC 
QMAKE_TARGET_DESCRIPTION = IrrerrC IRC client
QMAKE_TARGET_COMPANY = nilsding
QMAKE_TARGET_COPYRIGHT = 2015-2016 nilsding

COMMIT = (unknown)
exists ($$PWD/.git) {
    COMMIT = $$system(git --git-dir $$PWD/.git --work-tree $$PWD describe --always --tags)
}

bsd {
    LIBS=-L/usr/local/lib
}

!win32 {
DEFINES += APP_VERSION=\\\"$$join(VERSION, "\\ ")\\\" \
           APP_NAME=\\\"$$join(QMAKE_TARGET_PRODUCT, "\\ ")\\\" \
           APP_DESCRIPTION=\\\"$$join(QMAKE_TARGET_DESCRIPTION, "\\ ")\\\" \
           APP_COMPANY=\\\"$$join(QMAKE_TARGET_COMPANY, "\\ ")\\\" \
           APP_COPYRIGHT=\\\"\\(c\\)\\ $$join(QMAKE_TARGET_COPYRIGHT, "\\ ")\\\" \
           DEFAULT_FONT=\\\"monospace\\\" \
           COMMIT=\\\"$$join(COMMIT, "\\ ")\\\"
}
win32 {
DEFINES += APP_VERSION=\"\\\"$$join(VERSION, " ")\\\"\" \
           APP_NAME=\"\\\"$$join(QMAKE_TARGET_PRODUCT, " ")\\\"\" \
           APP_DESCRIPTION=\"\\\"$$join(QMAKE_TARGET_DESCRIPTION, " ")\\\"\" \
           APP_COMPANY=\"\\\"$$join(QMAKE_TARGET_COMPANY, " ")\\\"\" \
           APP_COPYRIGHT=\"\\\"(c) $$join(QMAKE_TARGET_COPYRIGHT, " ")\\\"\" \
           DEFAULT_FONT=\\\"Consolas\\\" \
           COMMIT=\"\\\"$$join(COMMIT, " ")\\\"\"
}

