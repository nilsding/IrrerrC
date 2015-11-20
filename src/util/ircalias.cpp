#include "ircalias.h"

IrcAlias::IrcAlias(QString alias, QString action, IrcAliasType type, QObject *parent) : QObject(parent),
    _alias(alias), _action(action), _type(type)
{

}

IrcAlias::~IrcAlias()
{

}

