#ifndef IRCMESSAGE_H
#define IRCMESSAGE_H

#include <QObject>
#include <QStringList>

class IrcMessage : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString prefix READ prefix WRITE setPrefix)
    Q_PROPERTY(QString command READ command WRITE setCommand)
    Q_PROPERTY(QStringList const *params READ params)
    Q_PROPERTY(QString trailing READ trailing WRITE setTrailing)

public:
    explicit IrcMessage(QString prefix = "", QString command = "", QString params = "", QObject *parent = 0);

    QString prefix() { return _prefix; }
    void setPrefix(QString prefix) { _prefix = prefix; }
    QString command() { return _command; }
    void setCommand(QString command) { _command = command; }
    QStringList const *params() { return &_params; }
    void setParams(QString params);
    QString trailing() { return _trailing; }
    void setTrailing(QString trailing) { _trailing = trailing; }

    QString target();

signals:

public slots:

private:
    QString _prefix;
    QString _command;
    QStringList _params;
    QString _trailing;

};

#endif // IRCMESSAGE_H
