#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QSignalMapper>
#include <QToolButton>
#include <QMessageBox>
#include <QCloseEvent>
#include <QTimer>

#include "core/ircconnection.h"
#include "core/ircmessage.h"
#include "core/irccodes.h"
#include "settings/nsettings.h"
#include "settings/settingsdialog.h"
#include "util/ircalias.h"
#ifdef SCRIPTING_ENABLED
#include "script/njsengine.h"
#include "script/nscriptaction.h"
#include "script/scriptlistingdialog.h"
#endif
#include "aboutdialog.h"
#include "statuswindow.h"

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
    StatusWindow *findMdiChild(StatusWindow::NWindowType type);

public slots:

    void loadSettings();

private slots:
    StatusWindow *createMdiChild(StatusWindow::NWindowType winType = StatusWindow::NWindowStatus);
    void updateWindowMenu();
    void selectActiveSubWindow(QWidget *);
    void updateToolsMenu();

    void on_qaConnect_triggered();
    void onWindowTextEntered(QString);
    void onNewMessageReceived(IrcMessage *);
    void onUserActivated(QString);

    void on_centralWidget_subWindowActivated(QMdiSubWindow *arg1);

    void on_qaSettings_triggered();

    void on_qaAboutQt_triggered();

    void on_qaAbout_triggered();

    void on_qaChannelList_triggered();

    void on_qaQuit_triggered();

    void on_qaScripts_triggered();

    void on_qaLoadScripts_triggered();

    void on_qaReloadScripts_triggered();

    void on_qaUnloadScripts_triggered();

private:
    Ui::MainWindow *_ui;

    IrcConnection *_conn;
    IrcIdentity *_id;
    QList<IrcNetwork *> _networks;
    QList<IrcAlias *> _aliases;

    QSignalMapper *_mapper;

    bool _debug;

    StatusWindow *activeMdiChild();

    void handleNumericResponseCode(IrcMessage *);
    void handleNonNumericResponse(IrcMessage *);
    void storeSettings();
};

#endif // MAINWINDOW_H
