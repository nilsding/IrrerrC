#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow), _conn(new IrcConnection), _id(new IrcIdentity), _net(new IrcNetwork),
    _mapper(new QSignalMapper(this))
{
    _ui->setupUi(this);
    _id->setNickname("qnilsding");
    _id->setRealname("nilsding (Qt testing)");
    _id->setInvisible(true);
    _id->setUsername("nIRC");
    _net->setName("rrerr.net test dings");
    _conn->setIdentity(_id);

    StatusWindow *status = createMdiChild();
    status->show();

    connect(_ui->qmWindow, SIGNAL(aboutToShow()), this, SLOT(updateWindowMenu()));
    connect(_mapper, SIGNAL(mapped(QWidget *)), this, SLOT(selectActiveSubWindow(QWidget *)));
    connect(_conn, SIGNAL(newMessageReceived(IrcMessage *)), this, SLOT(onNewMessageReceived(IrcMessage *)));
}

MainWindow::~MainWindow()
{
    delete _ui;
}

StatusWindow *MainWindow::createMdiChild(StatusWindow::NWindowType winType)
{
    auto win = new StatusWindow(winType);
    connect(win, SIGNAL(textEntered(QString)), this, SLOT(onWindowTextEntered(QString)));
    _ui->centralWidget->addSubWindow(win);
    return win;
}

void MainWindow::updateWindowMenu()
{
    _ui->qmWindow->clear();

    QList<QMdiSubWindow *> windows = _ui->centralWidget->subWindowList();

    for (int i = 0; i < windows.size(); i++) {
        StatusWindow *win = qobject_cast<StatusWindow *>(windows.at(i)->widget());
        QString text;
        if (i < 9) {
            text = tr("&%1 %2").arg(i + 1).arg(win->windowTitle());
        } else {
           text = tr("%1 %2").arg(i + 1).arg(win->windowTitle());
        }

        QAction *action  = _ui->qmWindow->addAction(text);
        action->setCheckable(true);
        action->setChecked(win == activeMdiChild());
        connect(action, SIGNAL(triggered()), _mapper, SLOT(map()));
        _mapper->setMapping(action, windows.at(i));
    }
}

void MainWindow::selectActiveSubWindow(QWidget *w)
{
    if (!w) {
        return;
    }
    _ui->centralWidget->setActiveSubWindow(qobject_cast<QMdiSubWindow *>(w));
}

void MainWindow::on_qaConnect_triggered()
{
    _conn->connectToHost(_net);
}

void MainWindow::onWindowTextEntered(QString s)
{
    _conn->raw(s);
}

void MainWindow::onNewMessageReceived(IrcMessage *msg)
{
    QString target = msg->target();
    if (target == "") {
        qobject_cast<StatusWindow *>(_ui->centralWidget->subWindowList().at(0)->widget())->receiveMessage(msg);
        return;
    }

    StatusWindow *win = findMdiChild(target);
    if (!win) {
        if (target.startsWith('#') || target.startsWith('&')) {
            win = createMdiChild(StatusWindow::NWindowChannel);
        } else {
            win = createMdiChild(StatusWindow::NWindowQuery);
        }
        win->setWindowTitle(target);
        win->show();
    }
    win->receiveMessage(msg);
}

StatusWindow *MainWindow::activeMdiChild()
{
    if (QMdiSubWindow *activeSubWindow = _ui->centralWidget->activeSubWindow()) {
        return qobject_cast<StatusWindow *>(activeSubWindow->widget());
    }
    return 0;
}

StatusWindow *MainWindow::findMdiChild(QString windowTitle, Qt::CaseSensitivity cs)
{
    QList<QMdiSubWindow *> windows = _ui->centralWidget->subWindowList();
    for (QMdiSubWindow *win : windows) {
        if (win->widget()->windowTitle().mid(0, win->widget()->windowTitle().indexOf(' ')).contains(windowTitle, cs)) {
            return qobject_cast<StatusWindow *>(win->widget());
        }
    }
    return 0;
}
