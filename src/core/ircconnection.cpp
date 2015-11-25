#include "ircconnection.h"

IrcConnection::IrcConnection(QObject *parent) :
    QObject(parent), _sock(new QTcpSocket()), _identity(0), _parser(new IrcParser(this))
{
    connect(_sock, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(_sock, &QTcpSocket::stateChanged, this, [=](QAbstractSocket::SocketState s) {
        emit(connectionStateChanged(s));
    });
}

IrcConnection::~IrcConnection()
{
    _sock->deleteLater();
    _parser->deleteLater();
    if (_identity != 0) {
        _identity->deleteLater();
    }
}

//!
//! \brief IrcConnection::connectToHost Connects to a given IRC network
//! \param network The IRC network to connect to
//! \param server Connect to a specific server in the IRC network.  If this is set to -1, a random server in the
//!   network's server list will be used.
//!
void IrcConnection::connectToHost(IrcNetwork *network, int server)
{
    if (network == 0) {
        return;
    }

    QString s = network->getServer(server);
    qDebug() << "Using server" << s;

    QString host = "";
    int port = 0;
    bool tls = false;

    host = s.left(s.indexOf(':'));
    s = s.mid(host.length() + 1);
    tls = s[0] == '+';
    port = s.toInt();

    connectToHost(host, port, tls);
}

//!
//! \brief IrcConnection::connectToHost Connects to a given server.
//! \param hostName The host name of the server
//! \param port The to be used port
//!
void IrcConnection::connectToHost(QString hostName, quint16 port, bool _useTls) {
    qDebug() << hostName << port << _useTls;

    if (_identity == 0) {
        qDebug() << "no identity given";
        return;
    }

    if (isConnected()) {  // don't connect if we're already connected to the server
        return;
    }

    _sock->connectToHost(hostName, port);
    if (!_sock->waitForConnected()) {
        qDebug() << "failed to connect:" << _sock->errorString();
        // emit connectionError
        return;
    }

    _sock->write(QString("NICK ").append(_identity->nickname()).append("\r\n").toStdString().c_str());
    _sock->write(QString("USER ")
                 .append(_identity->username())
                 .append(" ")
                 .append(QString::number(_identity->usermode()))
                 .append(" * :")
                 .append(_identity->realname())
                 .append("\r\n").toStdString().c_str());
}

void IrcConnection::disconnect(bool force)
{
    if (!isConnected()) return;
    _sock->write(QString("QUIT :").append(_identity->quitMessage()).append("\r\n").toStdString().c_str());
    if (force) {
        _sock->flush();
        _sock->close();
    }
}

//!
//! \brief IrcConnection::readyRead Reads new data from the server and appends it to IrcConnection::_recvbuf.
//!
//! The IrcConnection::parseLines() method will be called afterwards.
//!
void IrcConnection::readyRead()
{
    _recvbuf.append(_sock->readAll());
    parseLines();
}

//!
//! \brief IrcConnection::parseLines Parses read lines from the server.
//!
//! This method emits:
//!
//!   * IrcConnection::newMessageReceived(IrcMessage *)
//!
void IrcConnection::parseLines()
{
    QTimer::singleShot(0, this, [&]() {
        int index = -1;

        while ((index = _recvbuf.indexOf("\r\n")) != -1) {
            auto content = _recvbuf.left(index);
            _recvbuf = _recvbuf.mid(index + 2);

            if (IrcMessage *msg = _parser->parseLine(content)) {
                emit newMessageReceived(msg);
            }
    //        qDebug() << content;
        }
    });
}
