#ifndef NSCRIPTBINDINGS_H
#define NSCRIPTBINDINGS_H

#include <QObject>
#include <QMessageBox>
#include <QJSEngine>
#include <QJSValue>
#include "aboutdialog.h"
#include "njsengine.h"
#include "nscriptutils.h"
#include "core/ircconnection.h"
#include "core/ircidentity.h"

class NScriptBindings : public QObject
{
    Q_OBJECT
public:
    explicit NScriptBindings(QObject *parent = 0);

    Q_INVOKABLE bool init(QJSValue function);
    Q_INVOKABLE bool deinit(QJSValue function = QJSValue(QJSValue::NullValue));
    Q_INVOKABLE int alert(const QString &message, const QString &type = "information");
    Q_INVOKABLE void showAboutDialog();
    Q_INVOKABLE void addUserMenuAction(const QString &text, QJSValue function);
    Q_INVOKABLE void addToolsMenuAction(const QString &text, QJSValue function);

    Q_INVOKABLE void privmsg(const QString &target, const QString &text);
    Q_INVOKABLE void action(const QString &target, const QString &text);

signals:

public slots:
};

#endif // NSCRIPTBINDINGS_H
