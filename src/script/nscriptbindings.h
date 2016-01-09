#ifndef NSCRIPTBINDINGS_H
#define NSCRIPTBINDINGS_H

#include <QObject>
#include <QMessageBox>
#include <QJSEngine>
#include <QJSValue>
#include "aboutdialog.h"
#include "njsengine.h"
#include "nscriptutils.h"

class NScriptBindings : public QObject
{
    Q_OBJECT
public:
    explicit NScriptBindings(QObject *parent = 0);

    Q_INVOKABLE bool init(QJSValue function);
    Q_INVOKABLE bool deinit(QJSValue function = QJSValue(QJSValue::NullValue));
    Q_INVOKABLE int alert(const QString &message, const QString &type = "information");
    Q_INVOKABLE void showAboutDialog();

signals:

public slots:
};

#endif // NSCRIPTBINDINGS_H
