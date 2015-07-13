#include "ircnetwork.h"

IrcNetwork::IrcNetwork(QObject *parent) : QObject(parent),
    _name("Unknown network"), _servers(new QStringList("irc.rrerr.net:6667")), _active(false)
{

}

//!
//! \brief IrcNetwork::getServer returns a (random) server for connecting
//! \param index The
//!  index of the to be used server.  If it's -1 or the index is invalid, a random server will be
//!   chosen.
//! \return A server entry that looks like this: `irc.rrerr.net:6667` (plaintext connection) or `irc.rrerr.net:+6697`
//!   (TLS connection).  If an error occurrs, an empty string is returned.
//!
QString IrcNetwork::getServer(int index)
{
    if (_servers == 0 || _servers->isEmpty()) {
        return "";
    }

    if (index > _servers->length() - 1 || index < 0) {
        index = qrand() % _servers->length();
    }

    return _servers->at(index);
}

//!
//! \brief IrcNetwork::addServer adds a new server to the server list.  If the server is already in the list, it will
//!   not be added twice.
//! \param host The hostname of the server.
//! \param port The port of the server
//! \param useTls Enable TLS encryption for this server
//!
void IrcNetwork::addServer(QString host, int port, bool useTls)
{
    QString s = "";

    s.append(host);
    s.append(":");

    if (useTls) {
        s.append("+");
    }

    s.append(QString::number(port));

    if (!_servers->contains(s, Qt::CaseInsensitive)) {
        _servers->append(s);
    }
}
