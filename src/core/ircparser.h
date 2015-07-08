#ifndef IRCPARSER_H
#define IRCPARSER_H

#include <QObject>
#include <QDebug>
#include "ircconnection.h"
#include "ircmessage.h"

class IrcParser : public QObject
{
    Q_OBJECT
public:
    explicit IrcParser(QObject *parent = 0);
    IrcMessage *parseLine(QString);

signals:

public slots:

};

#endif // IRCPARSER_H
