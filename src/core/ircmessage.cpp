#include "ircmessage.h"

IrcMessage::IrcMessage(QString prefix, QString command, QString params, QObject *parent)
    : QObject(parent), _prefix(prefix), _command(command), _trailing("")
{
    setParams(params);
}

void IrcMessage::setParams(QString params) {
    // some people use a : in their channel name, which would fuck up the channel listing resulting in a crash.
    // therefore I have to use " :" instead of ' ' here.
    auto i = params.indexOf(" :");
    if (i != -1) {
        _trailing = params.mid(i + 2);
        params = params.left(i);
    }

    _params = params.split(' ', QString::SkipEmptyParts);
}

QString IrcMessage::target()
{
    QString target = _params.first();
    if (_command.contains("JOIN") ||
        _command.contains("PART") ||
        _command.contains("PRIVMSG")) {
        return target.length() == 0 ? "" : target;
    } else {
        return "";
    }
}
