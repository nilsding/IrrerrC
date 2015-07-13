#ifndef IRCCONNECTION_H
#define IRCCONNECTION_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>

#include "ircnetwork.h"
#include "ircidentity.h"
#include "ircparser.h"
#include "ircmessage.h"

class IrcParser;

//!
//! \brief The IrcConnection class is responsible for connecting to an IRC server and for receiving/sending messages
//! from/to the IRC server.
//!
//! It includes no logic how a received message should be handled.  For this, the IrcParser class is used.
//!
class IrcConnection : public QObject
{
    Q_OBJECT

public:
    explicit IrcConnection(QObject *parent = 0);
    ~IrcConnection();
    void connectToHost(IrcNetwork *network, int server = -1);
    void connectToHost(QString hostName, quint16 port, bool _useTls = false);

    void disconnect(bool force = false);

    IrcIdentity *identity() { return _identity; }
    void setIdentity(IrcIdentity *identity) { if (_identity != 0) { delete _identity; } _identity = identity; }

    bool isConnected() { return _sock->state() > QAbstractSocket::ConnectingState; }

    void raw(QString s)
    {
        _sock->write(s.toStdString().c_str());
        _sock->write("\r\n");
        _sock->flush();
    }

    void raw(IrcMessage *msg, bool del = false)
    {
        _sock->write(msg->command().toStdString().c_str());
        for (QString param : *msg->params()) {
            _sock->write(" ");
            _sock->write(param.toStdString().c_str());
        }
        if (!msg->trailing().isEmpty()) {
            _sock->write(" :");
            _sock->write(msg->trailing().toStdString().c_str());
        }
        _sock->write("\r\n");
        _sock->flush();

        if (del) {
            delete msg;
        }
    }

signals:
    void newMessageReceived(IrcMessage *);
    void connectionStateChanged(QAbstractSocket::SocketState);

public slots:

private:
    QTcpSocket *_sock;      //!< The TCP socket
    QString _lastError;     //!< String containing the last socket error
    QByteArray _recvbuf;    //!< A buffer.
    IrcIdentity *_identity; //!< The to be used identity.
    IrcParser *_parser;

    void parseLines();

private slots:
    void readyRead();
};

#endif // IRCCONNECTION_H
