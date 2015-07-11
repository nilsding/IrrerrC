#include "statuswindow.h"

StatusWindow::StatusWindow(NWindowType windowType, QWidget *parent) : QSplitter(parent), _windowType(windowType)
{
    this->setAttribute(Qt::WA_DeleteOnClose);
    createLayout();
    loadSettings();
}

StatusWindow::~StatusWindow()
{
    storeSettings();
    _qteBuffer->deleteLater();
    _qleInput->deleteLater();
}

void StatusWindow::receiveMessage(IrcMessage *msg)
{
    QString s = "";
    s += QDateTime::currentDateTime().toString("[HH:mm:ss]");
    s += " <";
    s += msg->prefix();
    s += "> ";
    switch (_windowType) {
        case NWindowChannel:
        case NWindowQuery: {
            s += msg->trailing();
            break;
        }
        default: {
            for (QString ss : *msg->params()) {
                s += ss;
                s += " ";
            }
        }
    }
    _qteBuffer->append(s);
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
        }
    }
    emit textEntered(input);
    _qleInput->clear();
}

void StatusWindow::onTextEdited(QString s)
{

}

void StatusWindow::createLayout()
{
    // create a read-only QTextEdit for the buffer
    _qteBuffer = new QTextEdit(this);
    _qteBuffer->setReadOnly(true);

    // create a QLineEdit object for input
    _qleInput = new QLineEdit(this);
    _qteBuffer->setFocusProxy(_qleInput);
    connect(_qleInput, SIGNAL(returnPressed()), this, SLOT(onTextEntered()));
    connect(_qleInput, SIGNAL(textEdited(QString)), this, SLOT(onTextEdited(QString)));

    // setting up the splitter widget
    QSplitter *splitter = new QSplitter(this);
    splitter->setHandleWidth(1);
    splitter->addWidget(_qteBuffer);
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
            break;
        }
        case NWindowQuery: {
            setWindowTitle(tr("Query"));
            setWindowIcon(QIcon(":/icons/query"));
            break;
        }
    }
}

void StatusWindow::loadSettings()
{
    _SETTINGS.beginGroup("StatusWindow");
        auto ff = _SETTINGS.value("fontFamily", "monospace").toString();
        _qleInput->setFont(QFont(ff));
        _qteBuffer->setFontFamily(ff);
    _SETTINGS.endGroup();
}

void StatusWindow::storeSettings()
{
    _SETTINGS.beginGroup("StatusWindow");
        _SETTINGS.setValue("fontFamily", _qteBuffer->fontFamily());
    _SETTINGS.endGroup();
}
