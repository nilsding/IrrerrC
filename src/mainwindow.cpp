#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow), _conn(new IrcConnection), _id(new IrcIdentity),
    _mapper(new QSignalMapper(this))
{
    _ui->setupUi(this);

    loadSettings();
    _conn->setIdentity(_id);

    StatusWindow *status = createMdiChild();
    status->show();

    connect(_ui->qmWindow, SIGNAL(aboutToShow()), this, SLOT(updateWindowMenu()));
    connect(_mapper, SIGNAL(mapped(QWidget *)), this, SLOT(selectActiveSubWindow(QWidget *)));
    connect(_conn, SIGNAL(newMessageReceived(IrcMessage *)), this, SLOT(onNewMessageReceived(IrcMessage *)));
    connect(_conn, &IrcConnection::connectionStateChanged, this, [=](QAbstractSocket::SocketState state) {
        if (_conn->isConnected()) {
            _ui->qaConnect->setText(tr("&Disconnect"));
            _ui->qaConnect->setIcon(QIcon(":/icons/disconnect"));
        } else {
            _ui->qaConnect->setText(tr("&Connect"));
            _ui->qaConnect->setIcon(QIcon(":/icons/connect"));
        }
    });

    updateWindowMenu();
}

MainWindow::~MainWindow()
{
    storeSettings();
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
    _ui->qtbWindows->clear();

    QList<QMdiSubWindow *> windows = _ui->centralWidget->subWindowList();

    for (int i = 0; i < windows.size(); i++) {
        StatusWindow *win = qobject_cast<StatusWindow *>(windows.at(i)->widget());
        QString text;
        if (i < 9) {
            text = tr("&%1 %2").arg(i + 1).arg(win->targetName().isEmpty() ? win->windowTitle() : win->targetName());
        } else {
           text = tr("%1 %2").arg(i + 1).arg(win->targetName().isEmpty() ? win->windowTitle() : win->targetName());
        }

        QAction *action  = _ui->qmWindow->addAction(text);
        action->setCheckable(true);
        action->setChecked(win == activeMdiChild());
        connect(action, SIGNAL(triggered()), _mapper, SLOT(map()));
        _mapper->setMapping(action, windows.at(i));

        QToolButton *btn = new QToolButton(_ui->qtbWindows);
        btn->setText(text.mid(text.indexOf(' ') + 1));
        btn->setIcon(win->windowIcon());
        btn->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
        btn->setFixedWidth(96);
        btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        btn->setCheckable(true);
        btn->setChecked(win == activeMdiChild());
        connect(btn, SIGNAL(clicked()), _mapper, SLOT(map()));
        _mapper->setMapping(btn, windows.at(i));
        _ui->qtbWindows->addWidget(btn);
    }
}

void MainWindow::selectActiveSubWindow(QWidget *w)
{
    if (!w) {
        return;
    }
    _ui->centralWidget->setActiveSubWindow(qobject_cast<QMdiSubWindow *>(w));
    updateWindowMenu();
}

void MainWindow::on_qaConnect_triggered()
{
    if (_conn->isConnected()) {
        _conn->disconnect();
        return;
    }
    for (IrcNetwork *net : _networks) {
        if (net->isActive()) {
            return _conn->connectToHost(net);
        }
    }
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
        handleNumericResponseCode(msg);
        return;
    }

    StatusWindow *win = findMdiChild(target);
    if (!win) {
        if (target.startsWith('#') || target.startsWith('&')) {
            win = createMdiChild(StatusWindow::NWindowChannel);
        } else {
            win = createMdiChild(StatusWindow::NWindowQuery);
        }
        win->setTargetName(target);
        win->show();
    }
    win->receiveMessage(msg);
    updateWindowMenu();
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

void MainWindow::handleNumericResponseCode(IrcMessage *msg)
{
    bool ok = false;
    int command = msg->command().toInt(&ok);
    if (!ok) {
        return;
    }
    switch (command) {
        case 331: { // RPL_NOTOPIC
            StatusWindow *win = findMdiChild(msg->params()->at(1));
            win->setTargetDescription("");
            break;
        }
        case 332: { // RPL_TOPIC
            StatusWindow *win = findMdiChild(msg->params()->at(1));
            win->setTargetDescription(msg->trailing());
            break;
        }
    }
}

void MainWindow::on_centralWidget_subWindowActivated(QMdiSubWindow *_win)
{
    updateWindowMenu();
}

void MainWindow::loadSettings()
{
    _SETTINGS.beginGroup("MainWindow");
        resize(_SETTINGS.value("size", QSize(640, 450)).toSize());
        move(_SETTINGS.value("pos", QPoint(200, 200)).toPoint());
    _SETTINGS.endGroup();

    _SETTINGS.beginGroup("Identity");
        _id->setNickname(_SETTINGS.value("nickname", "Lamer").toString());
        _id->setRealname(_SETTINGS.value("realname", "Unknown lamer").toString());
        _id->setWallops(_SETTINGS.value("wallops", false).toBool());
        _id->setInvisible(_SETTINGS.value("invisible", true).toBool());
        _id->setUsername(_SETTINGS.value("username", "Lamer").toString());
        _id->setQuitMessage(_SETTINGS.value("quitmessage", "That was the wrong button!").toString());
    _SETTINGS.endGroup();

    _networks.clear();
    int size = _SETTINGS.beginReadArray("Networks");
        for (int i = 0; i < size; i++) {
            _SETTINGS.setArrayIndex(i);
            IrcNetwork *network = new IrcNetwork;
            network->setName(_SETTINGS.value("name").toString());
            network->setServers(_SETTINGS.value("servers").toStringList());
            network->setActive(_SETTINGS.value("active").toBool());
            _networks.append(network);
        }
    _SETTINGS.endArray();
    if (_networks.empty()) {
        auto net = new IrcNetwork;
        net->setName("rrerr.net test dings");
        net->setActive(true);
        _networks.append(net);
        storeSettings();
    }
}

void MainWindow::storeSettings()
{
    _SETTINGS.beginGroup("MainWindow");
        _SETTINGS.setValue("size", size());
        _SETTINGS.setValue("pos", pos());
    _SETTINGS.endGroup();

    _SETTINGS.beginGroup("Identity");
        _SETTINGS.setValue("nickname", _id->nickname());
        _SETTINGS.setValue("realname", _id->realname());
        _SETTINGS.setValue("wallops", _id->hasWallops());
        _SETTINGS.setValue("invisible", _id->isInvisible());
        _SETTINGS.setValue("username", _id->username());
    _SETTINGS.endGroup();

    _SETTINGS.beginWriteArray("Networks");
        for (int i = 0; i < _networks.size(); ++i) {
            _SETTINGS.setArrayIndex(i);
            _SETTINGS.setValue("name", _networks.at(i)->name());
            _SETTINGS.setValue("servers", *_networks.at(i)->servers());
            _SETTINGS.setValue("active", _networks.at(i)->isActive());
        }
    _SETTINGS.endArray();
}

void MainWindow::on_qaSettings_triggered()
{
    storeSettings();
    SettingsDialog *dlg = new SettingsDialog(this);
    dlg->setModal(true);
    dlg->show();
    connect(dlg, SIGNAL(accepted()), this, SLOT(loadSettings()));
}

void MainWindow::on_qaAboutQt_triggered()
{
    QMessageBox::aboutQt(this, tr("About Qt"));
}

void MainWindow::on_qaAbout_triggered()
{
    QMessageBox::about(this, tr("About nIRC"), tr("<h3>nIRC</h3>&copy; 2015 nilsding"));
}
