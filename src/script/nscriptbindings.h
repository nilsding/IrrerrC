#ifndef NSCRIPTBINDINGS_H
#define NSCRIPTBINDINGS_H

#include <QObject>
#include <QMessageBox>
#include "aboutdialog.h"

class NScriptBindings : public QObject
{
    Q_OBJECT
public:
    explicit NScriptBindings(QObject *parent = 0);

    Q_INVOKABLE int alert(const QString& message, const QString& type = "information");
    Q_INVOKABLE void showAboutDialog();

signals:

public slots:
};

#endif // NSCRIPTBINDINGS_H
