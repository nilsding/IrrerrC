#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // seed qrand()
    qsrand((uint) QTime::currentTime().msec());

    // set up translations
    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    a.installTranslator(&qtTranslator);

    QTranslator myappTranslator;
    myappTranslator.load("nIRC_" + QLocale::system().name());
    a.installTranslator(&myappTranslator);

    // initialize and show main window
    MainWindow w;
    w.show();

    return a.exec();
}
