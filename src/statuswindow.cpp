#include "statuswindow.h"

StatusWindow::StatusWindow(NWindowType windowType, QWidget *parent) : QSplitter(parent), _windowType(windowType),
    _currentIdentity(0), _formatter(new IrcTextFormatter)
{
    this->setAttribute(Qt::WA_DeleteOnClose);
    createUserList();
    if (_windowType == NWindowList) {
        createChannelList();
    }
    createLayout();
    loadSettings();
}

StatusWindow::~StatusWindow()
{
    storeSettings();
    switch (_windowType) {
        case NWindowList: {
            _qtvChannels->deleteLater();
            break;
        }
        case NWindowChannel: {
            emit textEntered(QString("PART %1 :%2").arg(_targetName).arg(_currentIdentity
                                                                         ? _currentIdentity->partMessage()
                                                                         : ""));
        }
        default: {
            _qteBuffer->deleteLater();
        }
    }
    _qleInput->deleteLater();
    _qlvUsers->deleteLater();
    _formatter->deleteLater();
}

void StatusWindow::closeEvent(QCloseEvent *ev)
{
    if (_windowType != NWindowStatus) {
        ev->accept();
        return;
    }

    auto ret = QMessageBox::question(this, tr("Really quit?"), tr("Closing this window will quit %1.").arg(APP_NAME));
    if (ret == QMessageBox::Yes) {
        emit statusWindowClosing();
    }
    ev->ignore();
}

void StatusWindow::receiveMessage(IrcMessage *msg)
{
    if (_windowType == NWindowList) {
        return;
    }

    QString s = "";
    s += QDateTime::currentDateTime().toString(_SETTINGS.value("StatusWindow/timestampFormat", "[HH:mm:ss]").toString().replace('<', "&gt;"));
    s += " &lt;";
    s += msg->prefix().split("!")[0];
    s += "&gt; ";

    // we have to replace the '&' character entities with "&amp;" here because we apparently support HTML now
    // also, replace every space with "&nbsp;", just because.
    QString formattedTrailing = _formatter->parse(msg->trailing().replace('&', "&amp;").replace(' ', "&nbsp;"));
    switch (_windowType) {
        case NWindowChannel:
        case NWindowQuery: {
            s += formattedTrailing;
            break;
        }
        default: {
            for (QString ss : *msg->params()) {
                s += ss;
                s += " ";
            }
            s += formattedTrailing;
        }
    }
    _qteBuffer->moveCursor(QTextCursor::End);
    _qteBuffer->insertHtml(s + "<br />");
}

void StatusWindow::onTextEntered()
{
    QString input = _qleInput->text();
    if (_windowType == NWindowChannel || _windowType == NWindowQuery) {
        if (input.startsWith('/')) {
            // TODO: handle aliases
            input = input.mid(1);
        } else {
            input = QString("PRIVMSG %1 :%2").arg(_targetName).arg(input);
            IrcMessage *m = 0;
            if ((m = (new IrcParser)->parseLine(input))) {
                if (_currentIdentity) {
                    m->setPrefix(_currentIdentity->nickname());
                }
                receiveMessage(m);
            }
        }
    }
    emit textEntered(input);
    _qleInput->clear();
}

void StatusWindow::onTextEdited(QString /*s*/)
{

}

void StatusWindow::onUserActivated(QModelIndex i)
{
    emit userActivated(_userList.at(i.row()));
}

void StatusWindow::onNamesReply(const QStringList &lst)
{
    _tmpUserList.append(lst);
}

void StatusWindow::onEndOfNamesReply()
{
    _userList.clear();
    _userList.append(_tmpUserList);
    _tmpUserList.clear();
    _qlvUsers->setModel(new QStringListModel(_userList));
}

void StatusWindow::onListReply(QString channel, int userCount, QString topic)
{
    _tmpChannelList.append(IrcTypes::ListEntry(channel, userCount, topic));
}

void StatusWindow::onEndOfListReply()
{
    _channelList.clear();
    _channelList.append(_tmpChannelList);
    _tmpChannelList.clear();
    auto model = new ChannelListModel(&_channelList);
    auto oldModel = _qtvChannels->model();
    _qtvChannels->setModel(model);
    oldModel->deleteLater();
}

void StatusWindow::createLayout()
{
    if (_windowType != NWindowList) {
        // create a read-only QTextEdit for the buffer
        _qteBuffer = new QTextEdit(this);
        _qteBuffer->setReadOnly(true);
    }

    // create a QLineEdit object for input
    _qleInput = new QLineEdit(this);
    if (_windowType != NWindowList) {
        _qteBuffer->setFocusProxy(_qleInput);
    }
    connect(_qleInput, SIGNAL(returnPressed()), this, SLOT(onTextEntered()));
    connect(_qleInput, SIGNAL(textEdited(QString)), this, SLOT(onTextEdited(QString)));

    // setting up the splitter widget
    QSplitter *splitter = new QSplitter(this);
    splitter->setHandleWidth(1);
    if (_windowType == NWindowList) {
        splitter->addWidget(_qtvChannels);
    } else {
        splitter->addWidget(_qteBuffer);
    }
    splitter->addWidget(_qlvUsers);
    splitter->setStretchFactor(0, 5);
    splitter->setStretchFactor(1, 1);

    QWidget *container = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(container);

    layout->setSpacing(0);
    layout->setMargin(0);
    layout->addWidget(splitter);
    layout->addWidget(_qleInput);
    setHandleWidth(1);

    switch (_windowType) {
        case NWindowStatus: {
            setWindowTitle(tr("Status"));
            setWindowIcon(QIcon(":/icons/status"));
            _qlvUsers->setVisible(false);
            break;
        }
        case NWindowChannel: {
            setWindowTitle(tr("Channel"));
            setWindowIcon(QIcon(":/icons/channel"));
            break;
        }
        case NWindowList: {
            setWindowTitle(tr("Channel list"));
            setWindowIcon(QIcon(":/icons/chanlist"));
            _qleInput->setVisible(false);
            _qlvUsers->setVisible(false);
            break;
        }
        case NWindowQuery: {
            setWindowTitle(tr("Query"));
            setWindowIcon(QIcon(":/icons/query"));
            _qlvUsers->setVisible(false);
            break;
        }
    }
}

void StatusWindow::createUserList()
{
    _qlvUsers = new QListView(this);
    _qlvUsers->setFocusPolicy(Qt::NoFocus);
    _qlvUsers->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(_qlvUsers, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(onUserActivated(QModelIndex)));
}

void StatusWindow::createChannelList()
{
    _qtvChannels = new QTreeView(this);
    _qtvChannels->setModel(new ChannelListModel(new QList<IrcTypes::ListEntry>()));
    _qtvChannels->header()->setStretchLastSection(true);
    _qtvChannels->header()->setSectionsMovable(false);
    connect(_qtvChannels, &QTreeView::doubleClicked, this, [=](const QModelIndex &index) {
        emit textEntered(QString("JOIN %1").arg(_channelList.at(index.row()).channelName()));
    });
}

void StatusWindow::loadSettings()
{
    _SETTINGS.beginGroup("StatusWindow");
        auto ff = _SETTINGS.value("fontFamily", DEFAULT_FONT).toString();
        _qleInput->setFont(QFont(ff));
        if (_windowType != NWindowList) {
            _qteBuffer->setFontFamily(ff);
            _qteBuffer->setStyleSheet(QString("font-family: \"%0\";").arg(ff));
        }
    _SETTINGS.endGroup();
}

void StatusWindow::storeSettings()
{
    _SETTINGS.beginGroup("StatusWindow");
        if (_windowType != NWindowList) {
            _SETTINGS.setValue("fontFamily", _qleInput->font().family());
        }
    _SETTINGS.endGroup();
}
