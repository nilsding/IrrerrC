#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QSignalMapper>
#include <QToolButton>

#include "core/ircconnection.h"
#include "core/ircmessage.h"
#include "statuswindow.h"
#include "nsettings.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    StatusWindow *findMdiChild(QString windowTitle, Qt::CaseSensitivity cs = Qt::CaseSensitive);

private slots:
    StatusWindow *createMdiChild(StatusWindow::NWindowType winType = StatusWindow::NWindowStatus);
    void updateWindowMenu();
    void selectActiveSubWindow(QWidget *);

    void on_qaConnect_triggered();
    void onWindowTextEntered(QString);
    void onNewMessageReceived(IrcMessage *);

    void on_centralWidget_subWindowActivated(QMdiSubWindow *arg1);

private:
    Ui::MainWindow *_ui;

    IrcConnection *_conn;
    IrcIdentity *_id;
    IrcNetwork *_net;

    QSignalMapper *_mapper;

    StatusWindow *activeMdiChild();
    void handleNumericResponseCode(IrcMessage *);
    void loadSettings();
    void storeSettings();
};

#endif // MAINWINDOW_H
