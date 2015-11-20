#ifndef IRCALIAS_H
#define IRCALIAS_H

#include <QObject>

class IrcAlias : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString alias READ alias WRITE setAlias)
    Q_PROPERTY(QString action READ action WRITE setAction)
    Q_PROPERTY(IrcAliasType type READ type WRITE setType)

public:
    enum IrcAliasType {
        AliasSimple,  //!< Basic alias (e.g. "ns" -> "msg NickServ IDENTIFY")
        AliasScript   //!< Scripted alias (e.g. "slap" -> method `slap_user` in a script)
    };

    explicit IrcAlias(QString alias = "", QString action = "", IrcAliasType type = AliasSimple, QObject *parent = 0);
    ~IrcAlias();

    QString alias() const { return _alias; }
    void setAlias(QString alias) { _alias = alias; }
    QString action() const { return _action; }
    void setAction(QString action) { _action = action; }
    IrcAliasType type() const { return _type; }
    void setType(IrcAliasType type) { _type = type; }

private:
    QString _alias;
    QString _action;
    IrcAliasType _type;
};

#endif // IRCALIAS_H
