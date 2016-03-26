#ifndef NSCRIPTACTION_H
#define NSCRIPTACTION_H

#include <QAction>
#include <QJSValue>
#include "njsengine.h"

class NScriptAction : public QAction
{
    Q_OBJECT

    Q_PROPERTY(NJSEngine::ActionType type READ type)
    Q_PROPERTY(QJSValue function READ function)

public:
    explicit NScriptAction(NJSEngine::ActionType type, const QString &text, QJSValue function, QAction *parent = 0);

    NJSEngine::ActionType type() const { return _type; }
    QJSValue function() const { return _function; }

public slots:
    void trigger();

private:

    NJSEngine::ActionType _type;
    QJSValue _function;
};

#endif // NSCRIPTACTION_H
