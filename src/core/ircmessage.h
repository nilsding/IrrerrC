#ifndef IRCMESSAGE_H
#define IRCMESSAGE_H

#include <QObject>

class IrcMessage : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString prefix READ prefix WRITE setPrefix)
    Q_PROPERTY(QString command READ command WRITE setCommand)
    Q_PROPERTY(QString params READ params WRITE setParams)

public:
    explicit IrcMessage(QString prefix = "", QString command = "", QString params = "", QObject *parent = 0);

    QString prefix() { return _prefix; }
    void setPrefix(QString prefix) { _prefix = prefix; }
    QString command() { return _command; }
    void setCommand(QString command) { _command = command; }
    QString params() { return _params; }
    void setParams(QString params) { _params = params; }

    QString target();

signals:

public slots:

private:
    QString _prefix;
    QString _command;
    QString _params;

};

#endif // IRCMESSAGE_H
