#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow), _conn(new IrcConnection), _id(new IrcIdentity),
    _mapper(new QSignalMapper(this))
{
    _ui->setupUi(this);
    setUnifiedTitleAndToolBarOnMac(true);
    setWindowTitle(QString("%1 %2").arg(APP_NAME).arg(APP_VERSION));
    
#ifdef Q_OS_MAC
    _ui->centralWidget->setViewMode(QMdiArea::TabbedView);
    auto childWidgets = _ui->centralWidget->children();
    // hide tab bar
    for (auto widget : childWidgets) {
        if (QString(widget->metaObject()->className()) == "QTabBar") {
            auto tabbar = dynamic_cast<QTabBar *>(widget);
            tabbar->setVisible(false);
            // TODO: remove empty margin on top of the window
        }
    }
#endif

    loadSettings();
    _conn->setIdentity(_id);

    NJSEngine::init();
    //connect(_NSCRIPT_ENGINE_INSTANCE, SIGNAL(actionsChanged()), this, SLOT(rebuildScriptActionMenus()));
    _NSCRIPT_ENGINE_INSTANCE->setConnection(_conn);
    _NSCRIPT_ENGINE_INSTANCE->setIdentity(_id);
    QTimer::singleShot(0, 0, [&]() {
        _NSCRIPT_ENGINE_INSTANCE->loadScripts();
    });

    StatusWindow *status = createMdiChild();
    status->show();
    status->setCurrentIdentity(_id);
    status->setWindowState(Qt::WindowMaximized);

    connect(_ui->qmWindow, SIGNAL(aboutToShow()), this, SLOT(updateWindowMenu()));
    connect(_ui->qmTools, SIGNAL(aboutToShow()), this, SLOT(updateToolsMenu()));
    connect(_mapper, SIGNAL(mapped(QWidget *)), this, SLOT(selectActiveSubWindow(QWidget *)));
    connect(_conn, SIGNAL(newMessageReceived(IrcMessage *)), this, SLOT(onNewMessageReceived(IrcMessage *)));
    connect(_conn, &IrcConnection::connectionStateChanged, this, [=](QAbstractSocket::SocketState /*state*/) {
        if (_conn->isConnected()) {
            _ui->qaConnect->setText(tr("&Disconnect"));
            _ui->qaConnect->setIcon(QIcon(":/icons/disconnect"));
        } else {
            _ui->qaConnect->setText(tr("&Connect"));
            _ui->qaConnect->setIcon(QIcon(":/icons/connect"));
        }
    });
    connect(status, &StatusWindow::statusWindowClosing, this, [=]() {
        if (_conn->isConnected()) {
            _conn->disconnect();
        }
        qApp->quit();
    });

    updateWindowMenu();

    // first time starting IrrerrC?
    if (!NSettings::settingsFileExists()) {
        on_qaSettings_triggered();
    }
}

MainWindow::~MainWindow()
{
    storeSettings();
    delete _ui;
}

StatusWindow *MainWindow::createMdiChild(StatusWindow::NWindowType winType)
{
    auto win = new StatusWindow(winType);
#ifdef Q_OS_MAC
    win->setWindowFlags(Qt::FramelessWindowHint);
#endif
    win->setAliases(&_aliases);
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
            text = QString("&%1 %2").arg(i + 1).arg(win->targetName().isEmpty() ? win->windowTitle() : win->targetName());
        } else {
           text = QString("%1 %2").arg(i + 1).arg(win->targetName().isEmpty() ? win->windowTitle() : win->targetName());
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

void MainWindow::updateToolsMenu()
{
    _ui->qmTools->clear();
    auto actions = QList<QAction *>();
    // XXX: add new default actions inside the Tools menu here:
    actions << _ui->qaScripts;
    _ui->qmTools->addActions(actions);

    QList<NScriptAction *> *scriptActions = _NSCRIPT_ENGINE_INSTANCE->actions();
    if (!scriptActions->isEmpty()) {
        actions.clear();
        _ui->qmTools->addSeparator();
        for (NScriptAction *action : *scriptActions) {
            if (action->type() != NJSEngine::ActionType::ToolsMenu) {
                continue;
            }
            actions << action;
        }
        _ui->qmTools->addActions(actions);
    }

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
        QTimer::singleShot(0, this, [=]() {
            qobject_cast<StatusWindow *>(_ui->centralWidget->subWindowList().at(0)->widget())->receiveMessage(msg);
        });
        handleNumericResponseCode(msg);
        return;
    }

    bool isSelf = msg->prefix().left(msg->prefix().indexOf('!')) == _id->nickname();

    bool isChannel = target.startsWith('#') || target.startsWith('&');
    if (!isChannel) {
        target = msg->prefix().split("!")[0];
    }

    StatusWindow *win = findMdiChild(target);
    if (!win) {
        // ignore own PART if window does not exist (anymore)
        if (msg->command().toUpper() == "PART" && isSelf) {
            return;
        }
        if (isChannel) {
            win = createMdiChild(StatusWindow::NWindowChannel);
            connect(win, SIGNAL(userActivated(QString)), this, SLOT(onUserActivated(QString)));
        } else {
            win = createMdiChild(StatusWindow::NWindowQuery);
        }
        win->setCurrentIdentity(_id);
        win->setTargetName(target);
        win->show();
    }
    QTimer::singleShot(0, this, [=]() {
        win->receiveMessage(msg);
    });
    updateWindowMenu();
}

void MainWindow::onUserActivated(QString user)
{
    StatusWindow *win = findMdiChild(user);
    if (!win) {
        win = createMdiChild(StatusWindow::NWindowQuery);
        win->setCurrentIdentity(_id);
        win->setTargetName(user);
    }
    win->show();
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

//!
//! \brief MainWindow::findMdiChild returns the first MDI child of NWindowType `type`.
//! \param type the window type.
//! \return a pointer to the window if a window with this type exists, otherwise 0.
//!
StatusWindow *MainWindow::findMdiChild(StatusWindow::NWindowType type)
{
    QList<QMdiSubWindow *> windows = _ui->centralWidget->subWindowList();
    for (QMdiSubWindow *win : windows) {
        if (qobject_cast<StatusWindow *>(win->widget())->type() == type) {
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
        handleNonNumericResponse(msg);
        return;
    }
    switch (command) {
        case RPL_BOUNCE: {
            _conn->addSupportedByServer(msg->params());
            auto networkName = _conn->support("NETWORK");
            if (networkName != "__nil__") {
                StatusWindow *win = findMdiChild(StatusWindow::NWindowStatus);
                win->setTargetName(networkName);
                updateWindowMenu();
            }
            break;
        }
        case RPL_NOTOPIC: {
            StatusWindow *win = findMdiChild(msg->params()->at(1));
            if (!win) {
                break;
            }
            win->setTargetDescription("");
            break;
        }
        case RPL_TOPIC: {
            StatusWindow *win = findMdiChild(msg->params()->at(1));
            if (!win) {
                break;
            }
            win->setTargetDescription(msg->trailing());
            break;
        }
        case RPL_NAMREPLY: {
            // params: [=*@] #channel
            // = -- public channel
            // @ -- secret channel
            // * -- private channel
            // trailing: user1 user2 @user3 user4 +user5 user6 ...
            StatusWindow *win = findMdiChild(msg->params()->at(2));
            if (!win) {
                break;
            }
            win->onNamesReply(msg->trailing().split(" ", QString::SplitBehavior::SkipEmptyParts));
            break;
        }
        case RPL_ENDOFNAMES: {
            StatusWindow *win = findMdiChild(msg->params()->at(1));
            if (!win) {
                break;
            }
            win->onEndOfNamesReply();
            break;
        }
        case RPL_LIST: {
            // params: #channel user_count
            // trailing: topic
            int user_count = msg->params()->at(2).toInt(&ok);
            if (!ok) {
                break;
            }

            StatusWindow *win = findMdiChild(StatusWindow::NWindowList);
            if (!win) {
                win = createMdiChild(StatusWindow::NWindowList);
                win->show();
            }

            // channel, user count, topic
            win->onListReply(msg->params()->at(1), user_count, msg->trailing());
            break;
        }
        case RPL_LISTEND: {
            // trailing: "End of LIST" (or something like that)
            StatusWindow *win = findMdiChild(StatusWindow::NWindowList);
            if (!win) {
                break;
            }

            win->onEndOfListReply();
            break;
        }
    }
}

void MainWindow::handleNonNumericResponse(IrcMessage *msg)
{
    QString sourceNick = msg->prefix().left(msg->prefix().indexOf('!'));
    QString cmd = msg->command().toUpper();
    bool isSelf = sourceNick == _id->nickname();
    // handle nick change
    if (cmd == "QUIT") {
        // TODO: cycle through all subwindows (i.e. channels and query windows) and check if there is a user with the
        // source nick in it, then remove their nick and write a "User quit" message
        qDebug() << "User quit:" << sourceNick << "-- Reason:" << msg->trailing();
    } else if (cmd == "NICK") {
        if (isSelf) {
            _id->setNickname(msg->params()->first());
        }

        // TODO: cycle through all subwindows (i.e. channels and query windows) and check if there is a user with the
        // source nick in it, then change their nick and write a "nick changed" message
    }
}

void MainWindow::on_centralWidget_subWindowActivated(QMdiSubWindow */*win*/)
{
    updateWindowMenu();
}

void MainWindow::loadSettings()
{
    _SETTINGS.beginGroup("MainWindow");
        resize(_SETTINGS.value("size", QSize(640, 450)).toSize());
        move(_SETTINGS.value("pos", QPoint(200, 200)).toPoint());

        auto i = _SETTINGS.value("backgroundImage", "").toString();
        _ui->centralWidget->setBackground(i.isEmpty() ? _ui->centralWidget->palette().dark() : QBrush(QImage(i)));
#ifndef Q_OS_MAC
        _ui->qtbMain->setStyleSheet(QString(
            "QToolBar {"
                "background-image: url(") + _SETTINGS.value("MainToolbar/backgroundImage", "").toString() + "); }");
        _ui->qtbWindows->setStyleSheet(QString(
            "QToolBar {"
                "background-image: url(") + _SETTINGS.value("WindowToolbar/backgroundImage", "").toString() + "); }");
#endif
    _SETTINGS.endGroup();

    _SETTINGS.beginGroup("Identity");
        _id->setNickname(_SETTINGS.value("nickname", "Lamer").toString());
        _id->setRealname(_SETTINGS.value("realname", "Unknown lamer").toString());
        _id->setWallops(_SETTINGS.value("wallops", false).toBool());
        _id->setInvisible(_SETTINGS.value("invisible", true).toBool());
        _id->setUsername(_SETTINGS.value("username", "Lamer").toString());
        _id->setQuitMessage(_SETTINGS.value("quitmessage", "Connection reset by beer").toString());
        _id->setPartMessage(_SETTINGS.value("partmessage", "I can't think of a proper part message.").toString());
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

    _aliases.clear();
    size = _SETTINGS.beginReadArray("Aliases");
        for (int i = 0; i < size; i++) {
            _SETTINGS.setArrayIndex(i);
            IrcAlias *alias = new IrcAlias;
            alias->setType(static_cast<IrcAlias::IrcAliasType>(_SETTINGS.value("type").toUInt()));
            alias->setAlias(_SETTINGS.value("alias").toString());
            alias->setAction(_SETTINGS.value("action").toString());
            _aliases.append(alias);
        }
    _SETTINGS.endArray();

    QList<QMdiSubWindow *> windows = _ui->centralWidget->subWindowList();
    for (QMdiSubWindow *win : windows) {
        qobject_cast<StatusWindow *>(win->widget())->loadSettings();
    }

    _SETTINGS.beginGroup("Developer");
        _debug = _SETTINGS.value("debugMenusEnabled", false).toBool();
        _ui->qmDebug->menuAction()->setVisible(_debug);
    _SETTINGS.endGroup();
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
        _SETTINGS.value("quitmessage", _id->quitMessage());
        _SETTINGS.value("partmessage", _id->partMessage());
    _SETTINGS.endGroup();

    _SETTINGS.beginWriteArray("Networks");
        for (int i = 0; i < _networks.size(); ++i) {
            _SETTINGS.setArrayIndex(i);
            _SETTINGS.setValue("name", _networks.at(i)->name());
            _SETTINGS.setValue("servers", *_networks.at(i)->servers());
            _SETTINGS.setValue("active", _networks.at(i)->isActive());
        }
    _SETTINGS.endArray();

    _SETTINGS.beginWriteArray("Aliases");
        for (int i = 0; i < _aliases.size(); ++i) {
            _SETTINGS.setArrayIndex(i);
            _SETTINGS.setValue("type", _aliases.at(i)->type());
            _SETTINGS.setValue("alias", _aliases.at(i)->alias());
            _SETTINGS.setValue("action", _aliases.at(i)->action());
        }
    _SETTINGS.endArray();

    _SETTINGS.beginGroup("Developer");
        _SETTINGS.setValue("debugMenusEnabled", _debug);
    _SETTINGS.endGroup();
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
    // QMessageBox::about(this, tr("About %1").arg(APP_NAME), tr("<h3>%1</h3>&copy; 2015 nilsding").arg(APP_NAME));
    AboutDialog ad;
    ad.exec();
}

void MainWindow::on_qaChannelList_triggered()
{
    StatusWindow *win = findMdiChild(StatusWindow::NWindowList);
    if (!win) {
        win = createMdiChild(StatusWindow::NWindowList);
        win->show();
    }
    if (_conn->isConnected()) {
        _conn->raw("LIST");
    }
}

void MainWindow::on_qaQuit_triggered()
{
    if (_conn->isConnected()) {
        auto ret = QMessageBox::question(this, tr("Really quit?"),
                                         tr("You are connected to an IRC network.  Are you sure you want to quit %1?")
                                           .arg(APP_NAME));
        if (ret != QMessageBox::Yes) {
            return;
        }
        _conn->disconnect();
    }
    qApp->quit();
}

void MainWindow::on_qaScripts_triggered()
{
    ScriptListingDialog *dlg = new ScriptListingDialog(this);
    dlg->show();
    // TODO: connect signals emitted by ScriptListingDialog
}

void MainWindow::on_qaLoadScripts_triggered()
{
    _NSCRIPT_ENGINE_INSTANCE->loadScripts();
}

void MainWindow::on_qaReloadScripts_triggered()
{
    _NSCRIPT_ENGINE_INSTANCE->reloadScripts();
}

void MainWindow::on_qaUnloadScripts_triggered()
{
    _NSCRIPT_ENGINE_INSTANCE->unloadScripts();
}
