#include "ircmessage.h"

IrcMessage::IrcMessage(QString prefix, QString command, QString params, QObject *parent)
    : QObject(parent), _prefix(prefix), _command(command), _params(params)
{

}

QString IrcMessage::target()
{
    QString target = _params.mid(0, _params.indexOf(' '));
    if (_command.contains("JOIN") ||
        _command.contains("PART") ||
        _command.contains("PRIVMSG")) {
        return target.length() == 0 ? "" : target;
    } else {
        return "";
    }
}
